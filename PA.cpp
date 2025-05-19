#include <iostream>
#include <fstream>
#include "json.hpp"  
#include <string>

using json = nlohmann::json;
using namespace std;

#define MAX_user 10

int jumlah_user = 0;
string username, password, penjual[MAX_user], pwsell[MAX_user], nama, deskripsi, hppj, nohp, cari_nama;
int percobaan=0;    
int pilihan, stok, harga, berat, namabr, stokbr, hargabr, beratbr;
bool ketemu;
int jumlahtopup, dana = 0;

///////////////////////////////////////////////////////
/// DATA PENJUAL 
///
///
///////////////////////////////////////////////////////

void datapenjual(string* penjual, string* pwsell, int* jumlah_user, string *hppj, int* dana){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }

    data["penjual"].push_back({
        {"username", penjual[*jumlah_user]},
        {"password", pwsell[*jumlah_user]},
        {"nohp", *hppj},
        {"dana", *dana}
    });

    ofstream outFile("data_user.json");
    outFile << data.dump(4);
    outFile.close();
    
    cout << "Pendaftaran penjual berhasil!\n";
}



// Fungsi untuk menyimpan data pembeli ke file JSON
void datapembeli(string* username, string* password, string* nohp){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    
    data["pembeli"].push_back({
        {"username", *username},
        {"password", *password},
        {"nohp", *nohp}
    });
    
    ofstream outFile("data_user.json");
    outFile << data.dump(4);
    outFile.close();
    
    cout << "Pendaftaran pembeli berhasil!\n";
}

bool cekuser(){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    
    // cek pembeli
    for (const auto& user : data["pembeli"]) {
        if (user["username"] == username) {
            return true;
        }
    }

    // Cek di penjual
    for (const auto& seller : data["penjual"]) {
        if (seller["username"] == penjual[jumlah_user]) {
            return true;
        }
    }

    return false;
}

bool verifikasiuser(){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    
    // cek pembeli
    for (const auto& user : data["pembeli"]) {
        if (user["username"] == username && user["password"] == password && user["nohp"] == nohp) {
            return true;
        }
    }

    // Cek di penjual
    for (const auto& seller : data["penjual"]) {
        if (seller["username"] == penjual[jumlah_user] && seller["password"] == pwsell[jumlah_user] && seller["nohp"] == hppj) {
            return true;
        }
    }

    return false;
}

    // ----------------------------------------
    // |     OTW KELOMPOK TERBAIK             |
    // |                                      |
    // |         FITUR PENJUAL                |
    // |             GACOR                    |
    // |           BY AHNAF                   |
    // |--------------------------------------|

void lihatbarang(){ // MELIHAT BARANG (PEMBELI)
    ifstream file("data_barang.json");
    json data;
    file >> data;

    cout << "\n== Data barang penjual ==" << endl;
    for (const auto& barang : data["barang"]) {
        cout << "nama: " << barang["nama"] << endl;
        cout << "stok  : " << barang["stok"] << endl;
        cout << "berat: " << barang["berat"] << endl;
        cout << "harga: " << barang["harga"] << endl;
    }
}

void barangpenjual(string& nama, int& stok, int &berat, int &harga){ // MENAMBAH BARANG (PENJUAL)
    cout<<"masukkan nama barang"<<endl;
    cin>>nama;
    cout<<"masukkan stok barang dalam kg atau satuan"<<endl;
    cin>>stok;
    cout<<"masukkan berat barang/penjualan atau satuan barang/penjualan"<<endl;
    cin>>berat;
    cout<<"masukkan harga barang/penjualan atau /kg"<<endl;
    cin>>harga;

    json data;
    ifstream inFile("data_barang.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }

    data["barang"].push_back({
        {"nama", nama},
        {"stok", stok},
        {"berat", berat},
        {"harga", harga}
    });

    ofstream outFile("data_barang.json");
    outFile << data.dump(4);
    outFile.close();

    lihatbarang();
}

void editbarang (string* cari_nama) { // MENGEDIT BARANG (PENJUAL)
    ifstream inFile("data_barang.json");

    json data;
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_barang.json" << endl;
        return;
    }

    ketemu = false;
    cout << "\n=== Edit Data Barang ===" << endl;
    cout << "Masukkan nama barang yang ingin diubah: ";
    cin >> *cari_nama;

    for (auto& item : data["barang"]) {
        if (item["nama"] == *cari_nama) {
            ketemu = true;

            string nama_baru;
            int harga_baru, stok_baru;

            cout << "Masukkan nama baru: ";
            cin >> nama_baru;
            cout << "Masukkan harga baru: ";
            cin >> harga_baru;
            cout << "Masukkan stok baru: ";
            cin >> stok_baru;

            item["nama"] = nama_baru;
            item["harga"] = harga_baru;
            item["stok"] = stok_baru;

            break;
        }
    }

    if (!ketemu) {
        cout << "Barang dengan nama \"" << *cari_nama << "\" tidak ditemukan!!" << endl;
    } 
    
    else {
        ofstream outFile("data_barang.json");
        if (outFile.is_open()) {
            outFile << data.dump(4);
            outFile.close();
            cout << "Data barang berhasil diubah!!" << endl;
        } 
        else {
            cout << "Gagal menyimpan perubahan ke file data_barang.json" << endl;
        }
    }
}

void menghapusbarang(string* cari_nama, bool* ketemu) {
    ifstream inFile("data_barang.json");
    json data;

    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_barang.json" << endl;
        return;
    }

    auto& list = data["barang"];
    *ketemu = false;

    cout << "\n=== Hapus Data Barang ===" << endl;
    cout << "Masukkan nama barang yang ingin dihapus: ";
    cin >> *cari_nama;

    // for (auto it = data["barang"].begin(); it != data["barang"].end(); ++it) {
    //     if ((*it)["nama"] == *cari_nama) {
    //         data["barang"].erase(it);
    //         *ketemu = true;
    //         break;
    //     }
    // }

    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i]["nama"] == *cari_nama) {
            list.erase(list.begin() + i);
            *ketemu = true;
            break;
                
        }
    }

    if (*ketemu) {
        ofstream outFile("data_barang.json");
        if (outFile.is_open()) {
            outFile << data.dump(4);
            outFile.close();
            cout << "Data \"" << *cari_nama << "\" sudah terhapus." << endl;
        } else {
            cout << "Gagal menyimpan perubahan." << endl;
        }
    } else {
        cout << "Data \"" << *cari_nama << "\" tidak ditemukan." << endl;
    }
}

// void tarikuang(){ }


    // ----------------------------------------
    // |    KELOMPOK LAIN GPT ITU BANG        |
    // |             BY AHNAF                 |
    // |         FITUR PEMBELI                |
    // |            GACOR LE                  |
    // |                                      |
    // |--------------------------------------|


// ID = Pesanan biar gampang navigasi status
// Username
// Password
// NoHP
// Saldo = Dana Masuk keluar



void topup(string* username, string* password, string* nohp, int* jumlahtopup, int* dana){
    json data;
    ifstream inFile("data_user.json");

    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_user.json" << endl;
        return;
    }

    bool found = false;
    for (auto& user : data["pembeli"]) {
        if (user["username"] == *username && user["password"] == *password && user["nohp"] == *nohp) {
            cout << "Masukkan jumlah top up: ";
            cin >> *jumlahtopup;

            if (user.contains("dana") && user["dana"].is_number()) {
                user["dana"] = user["dana"].get<int>() + *jumlahtopup;
            } else {
                user["dana"] = *jumlahtopup;
            }

            *dana = user["dana"];
            found = true;
            break;
        }
    }

    if (found) {
        ofstream outFile("data_user.json");
        if (outFile.is_open()) {
            outFile << data.dump(4);
            outFile.close();
            cout << "Top up berhasil. Saldo sekarang: " << *dana << endl;
        } else {
            cout << "Gagal menyimpan ke file data_user.json" << endl;
        }
    } else {
        cout << "Data pengguna tidak ditemukan. Top up gagal." << endl;
    }
}

        




///////////////////////////////////////////////////////
/// LOGIN, REGISTER, MENU, DAN MAIN
///
///
///////////////////////////////////////////////////////

void regis(string *username, string* password,string* penjual, string* pwsell, int* jumlah_user, int &pilihan,string*nohp){
    cout << "\n=== Menu Pendaftaran ===" << endl;
    cout << "1. Daftar sebagai penjual" << endl;
    cout << "2. Daftar sebagai pembeli" << endl;
    cout << "3. Keluar" << endl;
    cout << "Pilihan Anda: ";
    cin >> pilihan;
    cin.ignore();
    switch (pilihan)
    {
    case 1:
        cout<<"masukkan username"<<endl;
        cin>>penjual[*jumlah_user];
        if(cekuser()){
            cout<<"username sudah terdaftar, silahkan cari username lain"<<endl;
            return;
        }
        cout<<"masukkan password"<<endl;
        cin>>pwsell[*jumlah_user];
        cout<<"masukkan no hp"<<endl;
        cin>>hppj;
        cin.ignore();
        datapenjual(penjual, pwsell, jumlah_user,&hppj,&dana);
        (*jumlah_user)++;
        break;
    
    case 2:
        cout << "Masukkan username: ";
        cin>>*username;
        if(cekuser()){
            cout<<"username sudah terdaftar, silahkan cari username lain"<<endl;
            return;
        }
        cout << "Masukkan password: ";
        cin>>*password;
        cout<<"masukkan no hp"<<endl;
        cin>>*nohp;
        cin.ignore();
        datapembeli(username, password,nohp);
    break;

    case 3:
        cout << "Keluar dari program." << endl;
    break;
    default:
        cout << "Pilihan tidak valid." << endl;
        break;
    }

}
bool login(string* username, string* password,string* penjual, string* pwsell, int* jumlah_user, int &pilihan, int &percobaan,string*nohp,string*hppj){
    cout << "selamat datang di menu login" << endl;
    cout << "pilih menu login" << endl;
    cout << "1. login sebagai penjual" << endl;
    cout << "2.login sebagai pembeli" << endl;
    cout << "keluar" << endl;
    cout << "pilihan anda: ";
    cin >> pilihan;

    switch (pilihan){  
    // ----------------------------------------
    // |                                      |
    // |                                      |
    // |           MENU PENJUAL               |
    // |                                      |
    // |                                      |
    // |--------------------------------------|
    case 1:
        cout <<"masukkan username" <<endl;
        cin >> penjual[*jumlah_user];
        cout <<"masukkan password" <<endl;
        cin >> pwsell[*jumlah_user];
        cout << "masukkan no hp" <<endl;
        cin >> *hppj;
        if (verifikasiuser()){
            do{
            cout<<"login berhasil"<<endl;
            cout<<"silhakan pilih menu penjual"<<endl;
            cout<<"1. tambah barang"<<endl;
            cout<<"2. lihat barang"<<endl;
            cout<<"3. edit barang"<<endl;
            cout<<"4. hapus barang"<<endl;
            cout<<"5. menarik saldo"<<endl;
            cout<<"6. menampilkan rating"<<endl;
            cout<<"7. keluar"<<endl;
            cout<<"masukkan pilihan: ";
            cin>>pilihan;
            switch (pilihan){
                case 1:
                    barangpenjual(nama, stok, berat, harga);
                    break;
                case 2:
                    lihatbarang();
                    break;
                case 3:
                    editbarang(&cari_nama);
                break;
                case 4:
                    menghapusbarang(&cari_nama,&ketemu);
                    break;
                case 5:
                    cout<<"nanti ada narik saldo";
                break;
                case 6:
                    cout<<"update, bintang 5"<<endl;
                break;
                case 7:
                    cout<<"terimakasih sudah menggunakan program ini"<<endl;
                break;
                default:
                cout<<"pilihan tidak valid"<<endl;
            }}while(pilihan!=7);
            return true;
        }
        else{
            cout<<"login gagal"<<endl;
            cout<<"username atau password salah"<<endl;
            return false;
        }
        break;

    // ----------------------------------------
    // |                                      |
    // |                                      |
    // |           MENU PEMBELI               |
    // |                                      |
    // |                                      |
    // |--------------------------------------|
    case 2:
        cout << "Masukkan username: ";
        cin>>*username;
        cout << "Masukkan password: ";
        cin>>*password;
        cout <<"masukkan no hp"<<endl;
        cin>>*nohp;
        cin.ignore();
        if (verifikasiuser()){
            do{
            cout<<"login berhasil"<<endl;
            cout<<"Silahkan pilih menu"<<endl;
            cout<<"1. Top up dana"<<endl;
            cout<<"2. Beli barang"<<endl;
            cout<<"3. Keluar"<<endl;
            cout<<"Masukkan pilihan: ";
            cin>>pilihan;
            switch (pilihan){
                case 1:
                    topup(username, password, nohp, &jumlahtopup, &dana);
                break;
                // case 2:
                //     beli(nama, stok, berat, harga);
                // break;
                case 3:
                cout<<"terimakasih sudah menggunakan program ini"<<endl;
                break;
                default:
                cout<<"pilihan tidak valid"<<endl;
            }
            return true;
        }while(pilihan!=7);
        }
        else{
            cout<<"login gagal"<<endl;
            cout<<"username atau password salah"<<endl;
            return false;
        }
        break;
    case 3:
        cout << "Keluar dari program." << endl;
        break;

    default:
        cout << "Pilihan tidak valid." << endl;
        break;
    }
    return 0;
}

void menuutama(){
    do {
        cout << "\n=== Menu Utama ===" << endl;
        cout << "1. Daftar Pengguna" << endl;
        cout << "2. login" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilihan Anda: ";
        cin >> pilihan;
    
        switch (pilihan) {
            case 1:
                regis (&username, &password, penjual, pwsell, &jumlah_user, pilihan,&nohp);
                break;
            case 2:
                login(&username, &password,penjual, pwsell, &jumlah_user, pilihan, percobaan, &nohp, &hppj);
                break;
            case 3:
                cout << "Keluar dari program." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 3);
}

int main() {
menuutama();
    return 0;
}