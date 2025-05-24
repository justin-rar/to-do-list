#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

struct Node
{
    char tugas[50];
    char tanggal[50];
    char status[50];
    char catatan[50];
    Node *kanan, *kiri;
};

Node *awal, *akhir, *bantu, *hapus;
const char *namaFile = "todolist.txt";

int listKosong()
{
    if (awal == NULL)
        return 1;
    else
        return 0;
}

void tambahTugas(char tugas[50], char tanggal[50], char status[50], char catatan[50])
{
    Node *NB = new Node;
    strcpy(NB->tugas, tugas);
    strcpy(NB->tanggal, tanggal);
    strcpy(NB->status, status);
    strcpy(NB->catatan, catatan);
    NB->kanan = nullptr;
    NB->kiri = nullptr;

    if (listKosong())
    {
        awal = akhir = NB;
    }
    else if (strcmp(tanggal, awal->tanggal) < 0)
    {
        // tambah di depan
        NB->kanan = awal;
        awal->kiri = NB;
        awal = NB;
    }
    else if (strcmp(tanggal, akhir->tanggal) >= 0)
    {
        // tambah di belakang
        akhir->kanan = NB;
        NB->kiri = akhir;
        akhir = NB;
    }
    else
    {
        // tambah di tengah, cari posisi yang sesuai
        Node *bantu = awal;
        while (bantu->kanan != nullptr && strcmp(bantu->kanan->tanggal, tanggal) <= 0)
        {
            bantu = bantu->kanan;
        }
        NB->kanan = bantu->kanan;
        if (bantu->kanan != nullptr)
        {
            bantu->kanan->kiri = NB;
        }
        NB->kiri = bantu;
        bantu->kanan = NB;
    }
}

void hapusTugas(char nama[50])
{
    if (listKosong())
    {
        cout << "Belum ada tugas" << endl;
        return;
    }

    Node *bantu = awal;
    while (bantu != nullptr && strcmp(bantu->tugas, nama) != 0)
    {
        bantu = bantu->kanan;
    }

    if (bantu == nullptr)
    {
        cout << "Tugas tidak ditemukan" << endl;
        return;
    }

    if (bantu == awal)
    {
        awal = bantu->kanan;
        if (awal)
            awal->kiri = nullptr;
        else
            akhir = nullptr;
    }
    else if (bantu == akhir)
    {
        akhir = bantu->kiri;
        if (akhir)
            akhir->kanan = nullptr;
    }
    else
    {
        bantu->kiri->kanan = bantu->kanan;
        bantu->kanan->kiri = bantu->kiri;
    }

    delete bantu;
    cout << "Tugas berhasil dihapus" << endl;
}

void tampilTugas()
{
    if (listKosong())
    {
        cout << "Tidak ada tugas" << endl;
        return;
    }

    Node *bantu = awal;
    while (bantu != nullptr)
    {
        cout << "Nama    : " << bantu->tugas << "\n";
        cout << "Tanggal : " << bantu->tanggal << "\n";
        cout << "Status  : " << bantu->status << "\n";
        cout << "Catatan : " << bantu->catatan << "\n";
        cout << "-----------------------------\n";
        bantu = bantu->kanan;
    }
}

void ubahStatus(char nama[50])
{
    Node *bantu = awal;
    while (bantu != nullptr && strcmp(bantu->tugas, nama) != 0)
    {
        bantu = bantu->kanan;
    }

    if (bantu == nullptr)
    {
        cout << "Tugas tidak ditemukan" << endl;
        return;
    }

    cout << "Status sekarang: " << bantu->status << "\n";
    cout << "Masukkan status baru (Belum / Proses / Selesai): ";
    cin.getline(bantu->status, 50);
    cout << "Status diperbarui" << endl;
}

void cariTugas(char caridata[50])
{
    if (listKosong())
    {
        cout << "Tidak ada tugas.\n";
        return;
    }

    Node *bantu = awal;
    bool found = false;

    while (bantu != nullptr)
    {
        if (strcmp(bantu->tugas, caridata) == 0)
        {
            cout << "Nama    : " << bantu->tugas << "\n";
            cout << "Tanggal : " << bantu->tanggal << "\n";
            cout << "Status  : " << bantu->status << "\n";
            cout << "Catatan : " << bantu->catatan << "\n";
            cout << "-----------------------------\n";
            found = true;
        }
        bantu = bantu->kanan;
    }

    if (!found){
        cout << "Tugas dengan " << caridata << " tidak ada" << endl;
    }
}

void simpanKeFile()
{
    FILE *ptr = fopen(namaFile, "w");
    if (ptr == NULL)
    {
        cout << "Gagal membuka file!" << endl;
        return;
    }

    Node *bantu = awal;
    while (bantu != nullptr)
    {
        fprintf(ptr, "%s\n%s\n%s\n%s\n", bantu->tugas, bantu->tanggal, bantu->status, bantu->catatan);
        bantu = bantu->kanan;
    }
    fclose(ptr);
}

void bacaDariFile()
{
    FILE *ptr = fopen(namaFile, "r");
    if (ptr == NULL)
    {
        cout << "Gagal membuka file!" << endl;
        return;
    }

    char tugas[50], tanggal[50], status[50], catatan[50];

    while (fscanf(ptr, "%49[^\n]\n%49[^\n]\n%49[^\n]\n%49[^\n]\n", tugas, tanggal, status, catatan) == 4)
    {
        tambahTugas(tugas, tanggal, status, catatan);
    }
    fclose(ptr);
}

void hapusSemuaNode()
{
    while (awal != nullptr)
    {
        Node *hapus = awal;
        awal = awal->kanan;
        delete hapus;
    }
    akhir = nullptr;
}

int main()
{
    awal = akhir = nullptr;
    int pilih;
    bacaDariFile();
    while (true)
    {
        system("cls");
        cout << "\nTO-DO LIST\n";
        cout << "==========\n";
        cout << "1. Tambah Tugas\n";
        cout << "2. Hapus Tugas\n";
        cout << "3. Lihat Semua Tugas\n";
        cout << "4. Ubah Status Tugas\n";
        cout << "5. Cari Tugas\n";
        cout << "6. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;
        cin.ignore();

        switch (pilih)
        {
        case 1:
        {
            system("cls");
            char tugas[50], tanggal[50], status[50], catatan[50];
            cout << "Nama Tugas                   : ";
            cin.getline(tugas, 50);
            cout << "Tanggal (YYYY-MM-DD)         : ";
            cin.getline(tanggal, 50);
            cout << "Status (Belum/Proses/Selesai): ";
            cin.getline(status, 50);
            cout << "Catatan                      : ";
            cin.getline(catatan, 50);
            tambahTugas(tugas, tanggal, status, catatan);
            simpanKeFile();
            break;
        }
        case 2:
        {
            char nama[50];
            system("cls");
            cout << "Masukkan nama tugas yang ingin dihapus: ";
            cin.getline(nama, 50);
            hapusTugas(nama);
            simpanKeFile();
            break;
        }
        case 3:
            system("cls");
            tampilTugas();
            system("pause");
            break;
        case 4:
        {
            char nama[50];
            system("cls");
            cout << "Masukkan nama tugas yang ingin diubah statusnya: ";
            cin.getline(nama, 50);
            ubahStatus(nama);
            simpanKeFile();
            break;
        }
        case 5:
        {
            char keyword[50];
            system("cls");
            cout << "Masukkan kata kunci untuk mencari tugas: ";
            cin.getline(keyword, 50);
            cariTugas(keyword);
            system("pause");
            break;
        }
        case 6:
            simpanKeFile();
            hapusSemuaNode();
            cout << "Keluar.\n";
            return 0;
        default:
            cout << "Pilihan tidak valid.\n";
        }
    }
}