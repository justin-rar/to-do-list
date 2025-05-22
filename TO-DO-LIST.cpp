#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

struct Node
{
    string tugas;
    string tanggal;
    string status;
    string catatan;
    Node *kanan, *kiri;
};

Node *awal, *akhir, *bantu, *hapus, *NB;
const char *namaFile = "todolist.txt";

int listKosong()
{
    if (awal == NULL)
        return 1;
    else
        return 0;
}

void tambahTugas(string tugas, string tanggal, string status, string catatan)
{
    Node *NB = new Node{tugas, tanggal, status, catatan, nullptr, nullptr};

    if (listKosong())
    {
        awal = akhir = NB;
    }
    else if (tanggal < awal->tanggal)
    {
        // tambah di depan
        NB->kanan = awal;
        awal->kiri = NB;
        awal = NB;
    }
    else if (tanggal >= akhir->tanggal)
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
        while (bantu->kanan != nullptr && bantu->kanan->tanggal <= tanggal)
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

void hapusTugas(string nama)
{
    Node *bantu = awal;
    while (bantu != nullptr && bantu->tugas != nama)
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

void ubahStatus(string nama)
{
    Node *bantu = awal;
    while (bantu != nullptr && bantu->tugas != nama)
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
    getline(cin, bantu->status);
    cout << "Status diperbarui" << endl;
}

void cariTugas(string keyword)
{
    if (listKosong())
    {
        cout << "Tidak ada tugas.\n";
        return;
    }

    Node *bantu = awal;
    bool ditemukan = false;

    while (bantu != nullptr)
    {
        if (bantu->tugas.find(keyword) != string::npos)
        {
            cout << "Nama    : " << bantu->tugas << "\n";
            cout << "Tanggal : " << bantu->tanggal << "\n";
            cout << "Status  : " << bantu->status << "\n";
            cout << "Catatan : " << bantu->catatan << "\n";
            cout << "-----------------------------\n";
            ditemukan = true;
        }
        bantu = bantu->kanan;
    }

    if (!ditemukan)
    {
        cout << "Tugas dengan kata kunci \"" << keyword << "\" tidak ditemukan.\n";
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
        fprintf(ptr, "%s|%s|%s|%s\n",
                bantu->tugas.c_str(),
                bantu->tanggal.c_str(),
                bantu->status.c_str(),
                bantu->catatan.c_str());
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

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), ptr))
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Hapus newline di akhir
        char *token = strtok(buffer, "|");
        if (!token)
            continue;

        string tugas = token;
        token = strtok(NULL, "|");
        if (!token)
            continue;
        string tanggal = token;
        token = strtok(NULL, "|");
        if (!token)
            continue;
        string status = token;
        token = strtok(NULL, "|");
        if (!token)
            continue;
        string catatan = token;

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
            string tugas, tanggal, status, catatan;
            cout << "Nama Tugas     : ";
            getline(cin, tugas);
            cout << "Tanggal (YYYY-MM-DD): ";
            getline(cin, tanggal);
            cout << "Status (Belum/Proses/Selesai): ";
            getline(cin, status);
            cout << "Catatan        : ";
            getline(cin, catatan);
            tambahTugas(tugas, tanggal, status, catatan);
            simpanKeFile();
            break;
        }
        case 2:
        {
            string nama;
            cout << "Masukkan nama tugas yang ingin dihapus: ";
            getline(cin, nama);
            hapusTugas(nama);
            simpanKeFile();
            break;
        }
        case 3:
            tampilTugas();
            system("pause");
            break;
        case 4:
        {
            string nama;
            cout << "Masukkan nama tugas yang ingin diubah statusnya: ";
            getline(cin, nama);
            ubahStatus(nama);
            simpanKeFile();
            break;
        }
        case 5:
        {
            string keyword;
            cout << "Masukkan kata kunci untuk mencari tugas: ";
            getline(cin, keyword);
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
