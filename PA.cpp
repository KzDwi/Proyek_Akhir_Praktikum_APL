#include <iostream>
#include <fstream>
#include "json.hpp"  
#include <string>
#include <set>

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

void tarikuang(string* username, string* password, string* nohp, int* dana) { // AI (PELAJARI DAN UBAH)
    json dataUser, dataBarang, dataPembelian;
    int totalTarik = 0;

    // Baca data pembelian
    ifstream inFilePembelian("data_pembelian.json");
    if (inFilePembelian.is_open()) {
        inFilePembelian >> dataPembelian;
        inFilePembelian.close();
    } else {
        cout << "Belum ada data pembelian.\n";
        return;
    }

    // Baca data penjual
    ifstream inFileUser("data_user.json");
    if (inFileUser.is_open()) {
        inFileUser >> dataUser;
        inFileUser.close();
    } else {
        cout << "Gagal membuka file data_user.json\n";
        return;
    }

    // Baca nama barang dari data_barang.json
    ifstream inFileBarang("data_barang.json");
    if (inFileBarang.is_open()) {
        inFileBarang >> dataBarang;
        inFileBarang.close();
    }

    // Kumpulkan penghasilan dari semua barang
    for (const auto& pembelian : dataPembelian["pembelian"]) {
        string namaBarang = pembelian["nama_barang"];
        int totalHarga = pembelian["total_harga"];

        // Cek apakah barang ini milik penjual saat ini
        for (const auto& barang : dataBarang["barang"]) {
            if (barang["nama"] == namaBarang) {
                // Dianggap semua barang di data_barang.json adalah milik penjual login saat ini
                totalTarik += totalHarga;
                break;
            }
        }
    }

    // Update saldo penjual
    for (auto& seller : dataUser["penjual"]) {
        if (seller["username"] == *username && seller["password"] == *password && seller["nohp"] == *nohp) {
            if (seller.contains("dana") && seller["dana"].is_number()) {
                seller["dana"] = seller["dana"].get<int>() + totalTarik;
            } else {
                seller["dana"] = totalTarik;
            }
            *dana = seller["dana"];
            break;
        }
    }

    // Simpan ke file
    ofstream outFile("data_user.json");
    outFile << dataUser.dump(4);
    outFile.close();

    cout << "Saldo berhasil ditarik dari pembelian: Rp" << totalTarik << endl;
    cout << "Saldo total penjual sekarang: Rp" << *dana << endl;
}


void laporanpenjualan(string* username) {
    json dataPembelian, dataBarang;
    map<string, pair<int, int>> laporan; // nama_barang -> <jumlah, total>

    // Baca data barang
    ifstream inFileBarang("data_barang.json");
    if (inFileBarang.is_open()) {
        inFileBarang >> dataBarang;
        inFileBarang.close();
    } else {
        cout << "Gagal membuka data_barang.json\n";
        return;
    }

    // Baca data pembelian
    ifstream inFilePembelian("data_pembelian.json");
    if (inFilePembelian.is_open()) {
        inFilePembelian >> dataPembelian;
        inFilePembelian.close();
    } else {
        cout << "Belum ada data pembelian.\n";
        return;
    }

    // Ambil semua barang milik penjual
    set<string> barangPenjual;
    for (const auto& barang : dataBarang["barang"]) {
        if (barang.contains("penjual") && barang["penjual"] == *username) {
            barangPenjual.insert(barang["nama"]);
        }
    }

    // Hitung total penjualan berdasarkan barang penjual
    for (const auto& beli : dataPembelian["pembelian"]) {
        string nama = beli["nama_barang"];
        int jumlah = beli["jumlah"];
        int total = beli["total_harga"];

        if (barangPenjual.count(nama)) {
            laporan[nama].first += jumlah;
            laporan[nama].second += total;
        }
    }

    // Tampilkan laporan
    cout << "\n=== Laporan Penjualan untuk Penjual: " << *username << " ===\n";
    if (laporan.empty()) {
        cout << "Belum ada penjualan untuk barang Anda.\n";
    } else {
        cout << left << setw(20) << "Nama Barang"
            << setw(10) << "Jumlah"
            << setw(15) << "Total Harga" << endl;
        cout << string(45, '-') << endl;

        for (const auto& entry : laporan) {
            cout << left << setw(20) << entry.first
                << setw(10) << entry.second.first
                << "Rp" << entry.second.second << endl;
        }
    }
}

    // ----------------------------------------
    // |    KELOMPOK LAIN GPT ITU BANG        |
    // |             BY AHNAF                 |
    // |         FITUR PEMBELI                |
    // |            GACOR LE                  |
    // |                                      |
    // |--------------------------------------|

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

        
void beli(string* username, string* password, string* nohp, int* dana) { // AI (PELAJARI DAN UBAH)
    json dataBarang, dataUser, dataPembelian;
    string namaBarang;
    int jumlahBeli;
    bool found = false;

    // Baca data barang
    ifstream inFileBarang("data_barang.json");
    if (inFileBarang.is_open()) {
        inFileBarang >> dataBarang;
        inFileBarang.close();
    } else {
        cout << "Gagal membuka data barang.\n";
        return;
    }

    // Baca data user
    ifstream inFileUser("data_user.json");
    if (inFileUser.is_open()) {
        inFileUser >> dataUser;
        inFileUser.close();
    } else {
        cout << "Gagal membuka data user.\n";
        return;
    }

    // Baca data pembelian (jika ada)
    ifstream inFilePembelian("data_pembelian.json");
    if (inFilePembelian.is_open()) {
        inFilePembelian >> dataPembelian;
        inFilePembelian.close();
    }

    // Mengimpor Dana dari data_user.json ke user
    bool found2 = false;
    for (auto& user : dataUser["pembeli"]) {
        if (user["username"] == *username && user["password"] == *password && user["nohp"] == *nohp) {
            *dana = user["dana"];
            found2 = true;
            break;
        }
    }
    
    // Tampilkan daftar barang
    cout << "\n=== Daftar Barang ===\n";
    for (const auto& barang : dataBarang["barang"]) {
        cout << "Nama: " << barang["nama"] << ", Harga: " << barang["harga"]
            << ", Stok: " << barang["stok"] << ", Berat: " << barang["berat"] << endl;
    }

    cout << "\nMasukkan nama barang yang ingin dibeli: ";
    cin >> namaBarang;
    for (auto& barang : dataBarang["barang"]) {
        if (barang["nama"] == namaBarang) {
            found = true;
            cout << "Masukkan jumlah yang ingin dibeli: ";
            cin >> jumlahBeli;

            int totalHarga = jumlahBeli * int(barang["harga"]);
            cout << "Total harga : " << totalHarga << endl;
            cout << "Dana Anda : " << *dana << endl;

            // Validasi
            if (jumlahBeli > int(barang["stok"])) {
                cout << "Stok tidak cukup!\n";
                return;
            }

            if (*dana < totalHarga) {
                cout << "Saldo tidak cukup!\n";
                return;
            }

            // Kurangi stok dan saldo
            barang["stok"] = int(barang["stok"]) - jumlahBeli;
            *dana -= totalHarga;

            // Update saldo pembeli di file
            for (auto& user : dataUser["pembeli"]) {
                if (user["username"] == *username && user["password"] == *password && user["nohp"] == *nohp) {
                    user["dana"] = *dana;
                    break;
                }
            }

            // Catat pembelian
            dataPembelian["pembelian"].push_back({
                {"username", *username},
                {"nama_barang", namaBarang},
                {"jumlah", jumlahBeli},
                {"total_harga", totalHarga},
                {"Status", "Dipesan"}
            });

            cout << "Pembelian berhasil! Total harga: " << totalHarga << ", Sisa saldo: " << *dana << endl;
            break;
        }
    }

    if (!found) {
        cout << "Barang tidak ditemukan.\n";
        return;
    }

    // Simpan perubahan ke file
    ofstream outFileBarang("data_barang.json");
    outFileBarang << dataBarang.dump(4);
    outFileBarang.close();

    ofstream outFileUser("data_user.json");
    outFileUser << dataUser.dump(4);
    outFileUser.close();

    ofstream outFilePembelian("data_pembelian.json");
    outFilePembelian << dataPembelian.dump(4);
    outFilePembelian.close();
}

// V Ubah Detail Pembeli (20 Mei 2025)
void editdetail(string *username, string *password, string *nohp) { // MENGEDIT DETAIL USER (PEMBELI)
    ifstream inFile("data_user.json");
    json data;
    bool verif = false;
    int verifAttempt = 0;

    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_user.json" << endl;
        return;
    }

    cout << "\n=== Edit Detail User ===" << endl;
    cout << "Masukkan Password Anda : ";
    cin >> *password;

    // Cari pengguna berdasarkan username dan password
    for (auto& detail : data["pembeli"]) {
        if (detail["username"] == *username && detail["password"] == *password) {
            verif = true;
            string nomor_baru, password_baru;

            cout << "[Ketik \"x\" jika tidak ingin dirubah]" << endl;
            cout << "Masukkan Password Baru: " << endl;
            cin >> password_baru;
            cout << "[Ketik \"x\" jika tidak ingin dirubah]" << endl;
            cout << "Masukkan Nomor HP Baru: " << endl;
            cin >> nomor_baru;
            cin.ignore();

            // Cek dan ubah detail
            if (password_baru != "x" && password_baru != " ") {
                detail["password"] = password_baru;
                }
            if (nomor_baru != "x" && nomor_baru != " ") {
                detail["nohp"] = nomor_baru;
                }

            ofstream outFile("data_user.json");
            if (outFile.is_open()) {
                outFile << data.dump(4);
                outFile.close();
                cout << "Detail berhasil diubah!" << endl;
                return;
                } 
            else {
                cout << "Gagal menyimpan perubahan ke file data_user.json" << endl;
                }
            return;
        }
    }

    // Loop Attempt
    while (!verif && verifAttempt < 3) {
        verifAttempt++;
        cout << "Password salah, silahkan ulangi kembali: ";
        cin >> *password;

        for (auto& detail : data["pembeli"]) {
            if (detail["username"] == *username && detail["password"] == *password) {
                verif = true;
                break; // Break Loop
            }
        }

        if (!verif && verifAttempt < 3) {
            cout << "Percobaan " << verifAttempt << " dari 3 gagal." << endl;
        }
    }
    if (!verif) {
        cout << "Terlalu banyak percobaan, kembali ke menu pembeli. . ." << endl;
    }
}


// ^ Ubah Detail Pembeli (20 Mei 2025)

void hapuspesanan(string *username, string *password){
    ifstream inFile1("data_pembelian.json");
    ifstream inFile2("data_user.json");
    json dataP, dataU;
    string cari_id;
    int pilihan;
    if (inFile1.is_open()){
        inFile1 >> dataP;
        inFile1.close();
    } else {
        cout << "Gagal membuka file data_pembelian.json" << endl;
        return;
    }
    if (inFile2.is_open()){
        inFile2 >> dataU;
        inFile2.close();
    } else {
        cout << "Gagal membuka file data_user.json" << endl;
        return;
    }

    cout << "\n== Data Pesanan ==" << endl;
    for (auto& pesan : dataP["pembelian"]) {
        if (pesan["username"] != *username){
            continue;
        }
        else {
            cout << "No. Pesanan: " << pesan["UID"] << endl;
            cout << "nama: " << pesan["nama_barang"] << endl;
            cout << "jumlah  : " << pesan["jumlah"] << endl;
            cout << "harga: " << pesan["total_harga"] << endl;
            cout << "status: " << pesan["Status"] << endl;
        }
    }
    cout << "[Masukkan 0 untuk kembali]" << endl;
    cout << "Pilih No. Pesanan yang ingin dihapus : " << endl;
    cin >> pilihan;
    auto& pesanan = dataP["pembelian"];
    bool terhapus = false;
            for (auto it = pesanan.begin(); it != pesanan.end(); ) {
                if ((*it)["UID"] == pilihan && (*it)["username"] == *username && (*it)["Status"] == "Dipesan") {
                    it = pesanan.erase(it); // Menghapus objek dan mendapatkan iterator baru
                    cout << "Pesanan dengan No. Pesanan " << pilihan << " telah dihapus" << endl;
                    terhapus = true;
                    break;
                } 
                else {
                    ++it;
                }
                break;
            }
            if (pilihan != 0 && !terhapus){
                cout << "Nomor Pesanan Tidak Sesuai" << endl;
            }
    ofstream outFile1("data_pembelian.json");
    outFile1 << dataP.dump(4);
    outFile1.close();
}

    // bool found = false;
    // for (auto& pesan : dataP["pembelian"]){
    //     if (pesan["UID"] == cari_id && pesan["username"] == *username){
    //         // 
    //     }
    // }



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
    cout << "2. login sebagai pembeli" << endl;
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
        cout << "masukkan username" <<endl;
        cin >> penjual[*jumlah_user];
        cout << "masukkan password" <<endl;
        cin >> pwsell[*jumlah_user];
        if (verifikasiuser()){
            cout<<"Berhasil Login ke Menu Penjual!"<<endl;
            do{
            cout<<"silahkan pilih menu penjual"<<endl;
            cout<<"1. tambah barang"<<endl;
            cout<<"2. lihat barang"<<endl;
            cout<<"3. edit barang"<<endl;
            cout<<"4. hapus barang"<<endl;
            cout<<"5. cek saldo"<<endl;
            cout<<"6. keluar"<<endl;
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
                    tarikuang(username, password, nohp, &dana);
                    break;
                case 6:
                    laporanpenjualan(&penjual[*jumlah_user]);
                    break;

                case 7:
                    cout<<"terimakasih sudah menggunakan program ini"<<endl;
                    return true;
                    break;
                default:
                cout<<"pilihan tidak valid"<<endl;
                }
            }while(pilihan!=7);
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
        cin.ignore();
        if (verifikasiuser()){
            cout<<"login berhasil"<<endl;
            do {
                cout<<"Silahkan pilih menu"<<endl;
                cout<<"1. Top up dana"<<endl;
                cout<<"2. Beli barang"<<endl;
                cout<<"3. Ubah Detail"<<endl;
                cout<<"4. Hapus barang"<<endl;
                cout<<"5. Keluar"<<endl;
                cout<<"Masukkan pilihan: ";
                cin>>pilihan;
                switch (pilihan){
                    case 1:
                        topup(username, password, nohp, &jumlahtopup, &dana);
                        continue;
                    case 2:
                        beli(username, password, nohp, &dana);
                        continue;
                    case 3:
                        editdetail(username, password, nohp);
                        continue;
                    case 4:
                        hapuspesanan(username, password);
                        continue;
                    case 5:
                        cout<<"Kembali ke menu Login"<<endl;
                        return true;
                    default:
                        cout<<"pilihan tidak valid"<<endl;
                }
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
