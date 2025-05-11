#include <iostream>
#include <fstream>
#include "json.hpp"  

using json = nlohmann::json;
using namespace std;

#define MAX_user 10

int jumlah_user = 0;
string username,password, penjual[MAX_user],pwsell[MAX_user],nama,deskripsi;
int percobaan=0;
int pilihan,stok,harga,berat;


void datapenjual(string* penjual, string* pwsell, int* jumlah_user){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }

    data["penjual"].push_back({
        {"username", penjual[*jumlah_user]},
        {"password", pwsell[*jumlah_user]}
    });

    ofstream outFile("data_user.json");
    outFile << data.dump(4);
    outFile.close();
    
    cout << "Pendaftaran penjual berhasil!\n";
}
// Fungsi untuk menyimpan data pembeli ke file JSON
void datapembeli(string* username, string* password){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    
    data["pembeli"].push_back({
        {"username", *username},
        {"password", *password}
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
        if (user["username"] == username && user["password"] == password) {
            return true;
        }
    }

    // Cek di penjual
    for (const auto& seller : data["penjual"]) {
        if (seller["username"] == penjual[jumlah_user] && seller["password"] == pwsell[jumlah_user]) {
            return true;
        }
    }

    return false;
}

void lihatbarang(){
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

void barangpenjual(string& nama, int& stok, int &berat, int &harga){
    cout<<"masukkan nama barang"<<endl;
    cin>>nama;
    cout<<"masukkan stok barang dalam kg"<<endl;
    cin>>stok;
    cout<<"masukkan berat barang/penjualan"<<endl;
    cin>>berat;
    cout<<"masukkan harga barang/penjualan"<<endl;
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

void regis(string *username, string* password,string* penjual, string* pwsell, int* jumlah_user, int &pilihan){
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
        datapenjual(penjual, pwsell, jumlah_user);
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
        datapembeli(username, password);
    break;
    case 3:
        cout << "Keluar dari program." << endl;
    break;
    default:
        cout << "Pilihan tidak valid." << endl;
        break;
    }

}
bool login(string* username, string* password,string* penjual, string* pwsell, int* jumlah_user, int &pilihan, int &percobaan){
    cout<<"selamat datang di menu login"<<endl;
    cout<<"pilih menu login"<<endl;
    cout<<"1. login sebagai penjual"<<endl;
    cout<<"2.login sebagai pembeli"<<endl;
    cout<<"keluar"<<endl;
    cout<<"pilihan anda: ";
    cin>>pilihan;
    switch (pilihan){
    case 1:
        cout<<"masukkan username"<<endl;
        cin>>penjual[*jumlah_user];
        cout<<"masukkan password"<<endl;
        cin>>pwsell[*jumlah_user];
        if (verifikasiuser()){
            do{
            cout<<"login berhasil"<<endl;
            cout<<"silhakan pilih menu penjual"<<endl;
            cout<<"1. tambah barang"<<endl;
            cout<<"2. lihat barang"<<endl;
            cout<<"masukkan pilihan: ";
            cin>>pilihan;
            switch (pilihan){
                case 1:
                    barangpenjual(nama, stok, berat, harga);
                    break;
                    case 2:
                    lihatbarang();
                    break;
            }}while(pilihan!=3);
            return true;
        }
        else{
            cout<<"login gagal"<<endl;
            cout<<"username atau password salah"<<endl;
            return false;
        }
        break;
    case 2:
        cout << "Masukkan username: ";
        cin>>*username;
        cout << "Masukkan password: ";
        cin>>*password;
        if (verifikasiuser()){
            cout<<"login berhasil"<<endl;
            cout<<"menu pembeli ini cuy"<<endl;
            return true;
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
    break;
    default:
        cout << "Pilihan tidak valid." << endl;
        break;
    }
        
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
                regis (&username, &password, penjual, pwsell, &jumlah_user, pilihan);
                break;
            case 2:
                login(&username, &password,penjual, pwsell, &jumlah_user, pilihan, percobaan);
                break;
            case 3:
                cout << "Keluar dari program." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
        }
    } while (pilihan != 4);
}

int main() {
menuutama();
    return 0;
}
