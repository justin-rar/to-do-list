#include <iostream>
using namespace std;

struct node {
    string info;
    node *kanan, *kiri;
};
node *awal, *akhir, *bantu, *hapus, *NB;

void buatlistbaru();
void sisipnode(char IB);
void hapusnode(char IH);
void bacamaju();
void bacamundur();
int listkosong();

int main() {
    cout << "tes kolaborasi";
    return 0;
}
