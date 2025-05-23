#include <iostream>
#include <fstream>
#include "json.hpp"  
#include <string>
#include <set>

using json = nlohmann::json;
using namespace std;

// ----------------------------------------
// |                                      |
// |           DEKLARASI                  |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

#define MAX_user 10
int jumlah_user = 0;
string username, usernameadmin, passwordadmin, password, penjual[MAX_user], pwsell[MAX_user], deskripsi, hppj, nohp, cari_nama, nama_toko;
int percobaan=0;    
int pilihan, namabr, stokbr, hargabr, beratbr;
bool ketemu;
int jumlahtopup, dana = 0;

struct InputBarang
{
    string namabarang;
    int stok;
    int harga;
    int berat;
};


// ----------------------------------------
// |                                      |
// |         FITUR CEK USER               |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

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
            usernameadmin = penjual[jumlah_user];
            passwordadmin = pwsell[jumlah_user];
            return true;
        }
    }

    return false;
}

// ----------------------------------------
// |                                      |
// |          FITUR ADMIN                 |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

void lihatbarang(string usernameadmin){ // MELIHAT BARANG PEMILIK TOKO ITU SENDIRI
    ifstream file("data_barang.json");
    json data;
    file >> data;

    bool found = false;
    cout << "\n== Data Toko " << usernameadmin << " ==" << endl;
    for (const auto& barang : data["barang"]) {
        if (barang["Toko"] == usernameadmin) { 
            cout << "nama: " << barang["nama"] << endl;
            cout << "stok: " << barang["stok"] << endl;
            cout << "berat: " << barang["berat"] << endl;
            cout << "harga: " << barang["harga"] << endl;
            cout << endl;
            found = true;

        }
    }

    if (found == false) {
        cout << "Tidak Ada Barang" << endl;
    }
    else {return;}
}

void lihatbarangpembeli() { // MELIHAT SEMUA BARANG YANG DIJUAL DARI BERBAGAI TOKO
    ifstream file("data_barang.json");
    json data;
    file >> data;
        
    cout << "\n== Data barang penjual ==" << endl;
    for (const auto& barang : data["barang"]) {
        cout << "Toko: " << barang["Toko"] << endl;
        cout << "nama: " << barang["nama"] << endl;
        cout << "stok: " << barang["stok"] << endl;
        cout << "berat: " << barang["berat"] << endl;
        cout << "harga: " << barang["harga"] << endl;
    }
}
    



void barangpenjual(string usernameadmin){ // MENAMBAH BARANG (PENJUAL)
    InputBarang TambahBarang;
    cin.ignore();
    cout << "masukkan nama barang"<<endl;
    getline(cin, TambahBarang.namabarang);
    cout << "masukkan stok barang dalam kg atau satuan"<<endl;
    cin >> TambahBarang.stok;
    cout << "masukkan berat barang/penjualan atau satuan barang/penjualan"<<endl;
    cin >> TambahBarang.berat;
    cout << "masukkan harga barang/penjualan atau /kg"<<endl;
    cin >> TambahBarang.harga;

    json data;
    ifstream inFile("data_barang.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }

    data["barang"].push_back({
        {"Toko", usernameadmin},
        {"nama", TambahBarang.namabarang},
        {"stok", TambahBarang.stok},
        {"berat", TambahBarang.berat},
        {"harga", TambahBarang.harga}
    });

    ofstream outFile("data_barang.json");
    outFile << data.dump(4);
    outFile.close();

    lihatbarang(usernameadmin);
}

void editbarang (string* cari_nama, string usernameadmin) { // MENGEDIT BARANG (PENJUAL)
    ifstream inFile("data_barang.json");

    json data;
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_barang.json" << endl;
        return;
    }

    // Menampilkan Data Barang
    lihatbarang(usernameadmin);

    // Memilih Barang Untuk Dihapus
    ketemu = false;
    cout << "\n=== Edit Data Barang ===" << endl;
    cout << "Masukkan nama barang yang ingin diubah: ";
    cin.ignore();
    getline (cin, *cari_nama);

    for (auto& item : data["barang"]) {
        if (item["nama"] == *cari_nama && item["Toko"] == usernameadmin) {
            ketemu = true;

            string nama_baru;
            int harga_baru, stok_baru;

            cout << "Masukkan nama baru: ";
            getline(cin, nama_baru);
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

void menghapusbarang(string* cari_nama, bool* ketemu, string usernameadmin) {
    ifstream inFile("data_barang.json");
    json data;

    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_barang.json" << endl;
        return;
    }

    // Menampilkan Barang
    lihatbarang(usernameadmin);

    auto& list = data["barang"];
    *ketemu = false;

    cout << "\n=== Hapus Data Barang ===" << endl;
    cout << "Masukkan nama barang yang ingin dihapus: ";
    cin.ignore();
    getline(cin, *cari_nama);

    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i]["nama"] == *cari_nama && list[i]["Toko"] == usernameadmin) {
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

void tarikuang(string usernameadmin, string passwordadmin, int* dana) { 
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
        for (auto& barang : dataPembelian["pembelian"]) {
            if (barang["Status"] == "Selesai" && barang["Toko"] == usernameadmin) {
                // Dianggap semua barang di data_barang.json adalah milik penjual login saat ini
                int totalHarga = barang["total_harga"];
                totalTarik += totalHarga;
            }
        }
    

    // Update saldo penjual
    for (auto& seller : dataUser["penjual"]) {
        if (seller["username"] == usernameadmin && seller["password"] == passwordadmin) {
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


void laporanpenjualan(string usernameadmin) { // Laporan Penjualan Jika Status Selesai
    int totalpenjualan = 0, penjualan;
    json dataPembelian;
    ifstream inFilePembelian("data_pembelian.json");
    if (inFilePembelian.is_open()) {
        inFilePembelian >> dataPembelian;
        inFilePembelian.close();
    } 
    else {
        cout << "Belum ada data pembelian.\n";
        return;
    }

    cout << "\n== LAPORAN PENJUALAN ==" << endl;
    cout << "Toko Admin: " << usernameadmin << endl;
    cout << "==================================" << endl;

    for (auto& data : dataPembelian["pembelian"])
    {
        if (data["Status"] == "Selesai" && data["Toko"] == usernameadmin) {
            cout << "Nama Pembeli: " << data["username"] << endl;
            cout << "Nama Barang: " << data["nama_barang"] << endl;
            cout << "Jumlah Barang: " << data["jumlah"] << endl;
            cout << "Total Harga: " << data["total_harga"] << endl;
            cout << "==================================" << endl;
        }
    }
    
    for (auto& data : dataPembelian["pembelian"])
    {
        if (data["Status"] == "Selesai" && data["Toko"] == usernameadmin) {
            penjualan = data["total_harga"];
            totalpenjualan += penjualan;
        }
    }

    cout << "Total Penghasilan Adalah: " << totalpenjualan << endl;
}

void konfirmasiPesananAdmin(string usernameadmin) { // Mengubah Status Konfirmasi Dari Dikirim menjadi Selsai
    ifstream inFile("data_pembelian.json");
    json data;
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_pembelian.json" << endl;
        return;
    }

    // Cek status untuk konfirmasi
    cout << "\n== Daftar Pesanan Anda Yang Perlu Konfirmasi ==" << endl;
    for (const auto& pesan : data["pembelian"]) {
        if (pesan["Toko"] == usernameadmin && pesan["Status"] == "Dikirim") {
            cout << "No. Pesanan: " << pesan["UID"] << endl;
            cout << "Nama: " << pesan["nama_barang"] << endl;
            cout << "Jumlah: " << pesan["jumlah"] << endl;
            cout << "Harga: " << pesan["total_harga"] << endl;
            cout << "Status: " << pesan["Status"] << endl;
            cout << endl;
        }
    }
    

    int noPesanan;
    cout << "Masukkan No. UID Pesanan yang ingin dikonfirmasi: ";
    cin >> noPesanan;

    bool found = false;
    for (auto& pesan : data["pembelian"]) {
        if (pesan["UID"] == noPesanan && pesan["Toko"] == usernameadmin) {
            if (pesan["Status"] == "Dikirim") {
                pesan["Status"] = "Selesai"; // Update status pesanan
                found = true;
                cout << "Pesanan dengan No. " << noPesanan << " telah dikonfirmasi." << endl;
                break;
            } else {
                cout << "Pesanan tidak dapat dikonfirmasi." << endl;
                return;
            }
        }
    }

    if (!found) {
        cout << "No. Pesanan tidak ditemukan." << endl;
    }

    // Simpan perubahan ke file
    ofstream outFile("data_pembelian.json");
    outFile << data.dump(4);
    outFile.close();
}
    // ----------------------------------------
    // |                                      |
    // |          FITUR PEMBELI               |
    // |            GACOR LE                  |
    // |                                      |
    // |--------------------------------------|

void konfirmasiPesanan(string* username) { // Mengkonfirmasi Pesanan Dari Dipesan Menjadi Dikirim Dan Menjadi Fungsi Rekursif Untuk Fungsi lihatpesanan()
    ifstream inFile("data_pembelian.json");
    json data;
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_pembelian.json" << endl;
        return;
    }

    // Cek status untuk konfirmasi
    cout << "\n== Daftar Pesanan Anda Yang Perlu Konfirmasi ==" << endl;
    for (const auto& pesan : data["pembelian"]) {
        if (pesan["username"] == *username && pesan["Status"] == "Dipesan") {
            cout << "No. Pesanan: " << pesan["UID"] << endl;
            cout << "Toko: " << pesan["Toko"] << endl;
            cout << "Nama: " << pesan["nama_barang"] << endl;
            cout << "Jumlah: " << pesan["jumlah"] << endl;
            cout << "Harga: " << pesan["total_harga"] << endl;
            cout << "Status: " << pesan["Status"] << endl;
            cout << endl;
        }
    }
    

    int noPesanan;
    cout << "Masukkan No. UID Pesanan yang ingin dikonfirmasi: ";
    cin >> noPesanan;

    bool found = false;
    for (auto& pesan : data["pembelian"]) {
        if (pesan["UID"] == noPesanan && pesan["username"] == *username) {
            if (pesan["Status"] == "Dipesan") {
                pesan["Status"] = "Dikirim"; // Update status pesanan
                found = true;
                cout << "Pesanan dengan No. " << noPesanan << " telah dikonfirmasi." << endl;
                break;
            } else {
                cout << "Pesanan tidak dapat dikonfirmasi." << endl;
                return;
            }
        }
    }

    if (!found) {
        cout << "No. Pesanan tidak ditemukan." << endl;
    }

    // Simpan perubahan ke file
    ofstream outFile("data_pembelian.json");
    outFile << data.dump(4);
    outFile.close();
}

void lihatpesanan(string* username) { // Melihat Pesanan Dan Ada Menu Tambahan
    ifstream inFile("data_pembelian.json");
    json data;
    int fitur;
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_pembelian.json" << endl;
        return;
    }

    cout << "\n== Daftar Pesanan Anda ==" << endl;
    for (const auto& pesan : data["pembelian"]) {
        if (pesan["username"] == *username) {
            cout << "No. Pesanan: " << pesan["UID"] << endl;
            cout << "Toko: " << pesan["Toko"] << endl;
            cout << "Nama: " << pesan["nama_barang"] << endl;
            cout << "Jumlah: " << pesan["jumlah"] << endl;
            cout << "Harga: " << pesan["total_harga"] << endl;
            cout << "Status: " << pesan["Status"] << endl;
            cout << endl;
        }
    }
    while (true)
    {
        cout << "1. Konfirmasi Pesanan" << endl;
        cout << "2. Lihat Pesanan (lagi)" << endl;
        cout << "3. Kembali ke menu" << endl;
        cout << "Masukkan pilihan : ";
        cin >> fitur;
        if (fitur == 1) {
            konfirmasiPesanan(username);
        }
        else if (fitur == 2) {
            for (const auto& pesan : data["pembelian"]) {
                if (pesan["username"] == *username) {
                cout << "No. Pesanan: " << pesan["UID"] << endl;
                cout << "Toko: " << pesan["Toko"] << endl;
                cout << "Nama: " << pesan["nama_barang"] << endl;
                cout << "Jumlah: " << pesan["jumlah"] << endl;
                cout << "Harga: " << pesan["total_harga"] << endl;
                cout << "Status: " << pesan["Status"] << endl;
                cout << endl;
                }
            }
        }
        else if (fitur == 3) {
            return;
        }
        else {
            cout << "Pilihan tidak valid" << endl;
        }
    }
}




void topup(string* username, string* password, string* nohp, int* jumlahtopup, int* dana, int &pilihan){ // Menambahkan Dana Ke User (pembeli)
    json data;
    ifstream inFile("data_user.json");
    int fitur;

    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_user.json" << endl;
        return;
    }

    bool found = false;
    for (auto& user : data["pembeli"]) {
        if (user["username"] == *username && user["password"] == *password) {
            cout << "============ TOP UP ============" << endl;
            cout << "[1] Pilih nominal top up" << endl;
            cout << "[2] Kustomisasi nominal top up" << endl;
            cout << "[3] Kembali ke menu" << endl;
            cout << "================================" << endl;
            cout << "Masukkan pilihan :" << endl;
            cin >> fitur;
            if (fitur == 1) {
                cout << "Pilih Nominal Top up" << endl;
                cout << "[1] Rp50rb  || [2] Rp100rb || [3] Rp250rb" << endl;
                cout << "[4] Rp500rb || [5] Rp1Jt   || [6] Rp1,5Jt" << endl;
                cin >> pilihan;
                switch (pilihan) {
                case 1:
                    *jumlahtopup = 50000;
                    break;
                case 2:
                    *jumlahtopup = 100000;
                    break;
                case 3:
                    *jumlahtopup = 250000;
                    break;
                case 4:
                    *jumlahtopup = 500000;
                    break;
                case 5:
                    *jumlahtopup = 1000000;
                    break;
                case 6:
                    *jumlahtopup = 1500000;
                    break;
                default:
                    cout << "Pilihan tidak valid" << endl;
                }
                if (found = true){
                    user["dana"] = user["dana"].get<int>() + *jumlahtopup;
                    *dana = user["dana"];
                    break;
                }
            }
            else if (fitur == 2) {
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
            else if (fitur == 3) {
                break;
            }
            else {
                cout << "Pilihan tidak valid" << endl;
            }
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
    cout << endl;
}
// 
        
void beli(string* username, string* password, string* nohp, int* dana) { // User Membeli Barang Dan Status Barang Dipesan
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
        if (user["username"] == *username && user["password"] == *password) {
            *dana = user["dana"];
            found2 = true;
            break;
        }
    }
    
    // Tampilkan daftar barang
    lihatbarangpembeli();

    int maxUID = 0;
    for (const auto& pesan : dataPembelian["pembelian"]) {
        int currentUID = pesan["UID"];
        if (currentUID > maxUID) {
            maxUID = currentUID;
        }
    }

    cout << "\nMasukkan nama barang yang ingin dibeli: ";
    cin.ignore();
    getline(cin, namaBarang);
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
                if (user["username"] == *username && user["password"] == *password) {
                    user["dana"] = *dana;
                    break;
                }
            }

            // Masukkan Nama Toko Ke Dalam Data Barang
            nama_toko = barang["Toko"];

            // Catat pembelian
            maxUID++;
            dataPembelian["pembelian"].push_back({
                {"UID", maxUID},
                {"Toko", nama_toko},
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

void editdetail(string *username, string *password, string *nohp) { // Mengedit Detail Pembeli
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

void hapuspesanan(string *username, string *password){ // Menghapus Pesanan Yang Telah Dipesan
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

// ----------------------------------------
// |                                      |
// |      REGIS, LOGIN DAN MENU           |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

void regis(string *username, string* password,string* penjual, string* pwsell, int* jumlah_user, int &pilihan,string*nohp){ // Registrasi
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
        cout << endl;
        cout<<"Masukkan username: ";
        cin>>penjual[*jumlah_user];
        if(cekuser()){
            cout<<"username sudah terdaftar, silahkan cari username lain"<<endl;
            return;
        }
        cout<<"Masukkan password: ";
        cin>>pwsell[*jumlah_user];
        cout<<"Masukkan No.hp: ";
        cin>>hppj;
        cin.ignore();
        datapenjual(penjual, pwsell, jumlah_user,&hppj,&dana);
        (*jumlah_user)++;
        break;
    
    case 2:
        cout << endl;
        cout << "Masukkan username: ";
        cin>>*username;
        if(cekuser()){
            cout<<"username sudah terdaftar, silahkan cari username lain"<<endl;
            return;
        }
        cout << "Masukkan password: ";
        cin>>*password;
        cout<<"masukkan No.hp: ";
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
bool login(string* username, string* password,string* penjual, string* pwsell, int* jumlah_user, int &pilihan, int &percobaan,string*nohp,string*hppj){ // Login
    cout << "\n=== Menu Login ==" << endl;
    cout << "1. Login sebagai penjual" << endl;
    cout << "2. Login sebagai pembeli" << endl;
    cout << "3. Keluar" << endl;
    cout << "Pilihan anda: ";
    cin >> pilihan;

    switch (pilihan){  
////// ----------------------------------------
////// |                                      |
////// |                                      |
////// |           MENU PENJUAL               |
////// |                                      |
////// |                                      |
////// |--------------------------------------|
    case 1:
        cout << "Masukkan username: ";
        cin >> penjual[*jumlah_user];
        cout << "Masukkan password: ";
        cin >> pwsell[*jumlah_user];
        if (verifikasiuser()){
            cout<<"Berhasil Login ke Menu Penjual!"<<endl;
            do{
            cout<<"\n=== Menu Penjual / Admin ==="<<endl;
            cout<<"1. tambah barang"<<endl;
            cout<<"2. lihat barang"<<endl;
            cout<<"3. edit barang"<<endl;
            cout<<"4. hapus barang"<<endl;
            cout<<"5. Cek Penghasilan"<<endl;
            cout<<"6. Laporan Penjualan"<<endl;
            cout<<"7. Cek Pesanan Pembeli"<<endl;
            cout<<"8. Keluar"<<endl;
            cout<<"Masukkan Pilihan: ";
            cin>>pilihan;
            switch (pilihan){
                case 1:
                    barangpenjual(usernameadmin);
                    break;
                case 2:
                    lihatbarang(usernameadmin);
                    break;
                case 3:
                    editbarang(&cari_nama, usernameadmin);
                    break;
                case 4:
                    menghapusbarang(&cari_nama,&ketemu, usernameadmin);
                    break;
                case 5:
                    tarikuang(usernameadmin, passwordadmin, &dana);
                    break;
                case 6:
                    laporanpenjualan(usernameadmin);
                    break;
                case 7:
                    konfirmasiPesananAdmin(usernameadmin);
                    break;
                case 8:
                    cout<<"terimakasih sudah menggunakan program ini"<<endl;
                    return true;
                    break;
                default:
                cout<<"pilihan tidak valid"<<endl;
                }
            }while(pilihan < 9);
        }
        else{
            cout<<"login gagal"<<endl;
            cout<<"username atau password salah"<<endl;
            return false;
        }
        break;

////// ----------------------------------------
////// |                                      |
////// |                                      |
////// |           MENU PEMBELI               |
////// |                                      |
////// |                                      |
////// |--------------------------------------|
    case 2:
        cout << "Masukkan username: ";
        cin>>*username;
        cout << "Masukkan password: ";
        cin>>*password;
        cin.ignore();
        if (verifikasiuser()){
            cout<<"login berhasil"<<endl;
            do {
                cout<<"\n=== Menu Pembeli / User ==="<<endl;
                cout<<"1. Top up dana"<<endl;
                cout<<"2. Beli barang"<<endl;
                cout<<"3. Ubah Detail"<<endl;
                cout<<"4. Hapus Pesanan"<<endl;
                cout<<"5. Lihat Pesanan"<<endl;
                cout<<"6. Keluar"<<endl;
                cout<<"Masukkan Pilihan: ";
                cin>>pilihan;
                switch (pilihan){
                    case 1:
                        topup(username, password, nohp, &jumlahtopup, &dana, pilihan);
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
                        lihatpesanan(username);
                        continue;
                    case 6:
                        cout<<"Kembali ke menu Login"<<endl;
                        return true;
                    default:
                        cout<<"pilihan tidak valid"<<endl;
                }
            }while(pilihan > 7);
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

void menuutama(){ // Menu Utama
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

// ----------------------------------------
// |                                      |
// |              MAIN                    |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

int main() { 
menuutama();
    return 0;
}
