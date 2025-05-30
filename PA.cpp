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
void menuutama();

struct InputBarang
{
    string namabarang;
    int stok;
    int harga;
    float berat;
};

// Bersihin Konsol/Terminal
int bebersih(){

    for (int i = 0; i < 25; i++)
    {
    cout << endl;
    cout << endl;
    }

    return 0;
}

// ----------------------------------------
// |                                      |
// |         FITUR CEK USER               |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

void datapenjual(string* penjual, string* pwsell, int& jumlah_user, const string& hppj, int dana){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }

    data["penjual"].push_back({
        {"username", penjual[jumlah_user]},
        {"password", pwsell[jumlah_user]},
        {"nohp", hppj},
        {"dana", dana}
    });

    ofstream outFile("data_user.json");
    outFile << data.dump(4);
    outFile.close();
    
    cout << "Pendaftaran penjual berhasil!\n";
}



// Fungsi untuk menyimpan data pembeli ke file JSON
void datapembeli(const string& username, const string& password, const string& nohp){
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    
    data["pembeli"].push_back({
        {"username", username},
        {"password", password},
        {"nohp", nohp}
    });
    
    ofstream outFile("data_user.json");
    outFile << data.dump(4);
    outFile.close();
    
    cout << "Pendaftaran pembeli berhasil!\n";
}

bool cekuser(){
    json data;
    ifstream inFile("data_user.json");

    string usernameadmin2 = penjual[jumlah_user];
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
        if (seller["username"] == usernameadmin2) {
            return true;
        }
    }

    return false;
}

bool verifikasiPembeli(const json& data, const string& username, const string& password) {
    for (const auto& user : data["pembeli"]) {
        if (user["username"] == username && user["password"] == password) {
            return true;
        }
    }
    return false;
}
bool verifikasiPenjual(const json& data, const string& username, const string& password) {
    for (const auto& seller : data["penjual"]) {
        if (seller["username"] == username && seller["password"] == password) {
            return true;
        }
    }
    return false;
}
bool verifikasiuser(const string& username, const string& password) {
    json data;
    ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    if (verifikasiPembeli(data, username, password)) {
        return true;
    }
    if (verifikasiPenjual(data, username, password)) {
        return true;
    }
    return false;
}


//Merge Sort
void merge(json& data, int sisikiri, int titiktengah, int sisikanan, const string& sortir, bool ascending) {
    int n1 = titiktengah - sisikiri + 1;
    int n2 = sisikanan - titiktengah;

    // Array sementara disini
    json Kiri = json::array();
    json Kanan = json::array();

    // Salin Data ke array
    for (int i = 0; i < n1; i++)
        Kiri.push_back(data["barang"][sisikiri + i]);
    for (int j = 0; j < n2; j++)
        Kanan.push_back(data["barang"][titiktengah + 1 + j]);

    // Merge (Ascending/Descending)
    int i = 0, j = 0, k = sisikiri;
    while (i < n1 && j < n2) {
        bool kondisi;
        if (sortir == "nama") {
            string Nilai1 = Kiri[i][sortir].get<string>();
            string Nilai2 = Kanan[j][sortir].get<string>();
            kondisi = (ascending) ? 
            (Nilai1 <= Nilai2) : (Nilai1 >= Nilai2);
        } else if (sortir == "harga") {
            int Nilai1 = Kiri[i][sortir].get<int>();
            int Nilai2 = Kanan[j][sortir].get<int>();
            kondisi = (ascending) ? (Nilai1 <= Nilai2) : (Nilai1 >= Nilai2);
        } else if (sortir == "stok") {
            int Nilai1 = Kiri[i][sortir].get<int>();
            int Nilai2 = Kanan[j][sortir].get<int>();
            kondisi = (ascending) ? (Nilai1 <= Nilai2) : (Nilai1 >= Nilai2);
        }
        if (kondisi) {
            data["barang"][k] = Kiri[i];
            i++;
        } else {
            data["barang"][k] = Kanan[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        data["barang"][k] = Kiri[i];
        i++;
        k++;
    }
    while (j < n2) {
        data["barang"][k] = Kanan[j];
        j++;
        k++;
    }
}

void mergeSort(json& data, int sisikiri, int sisikanan, const string& sortir, bool ascending) {
    if (sisikiri < sisikanan) {
        int titiktengah = sisikiri + (sisikanan - sisikiri) / 2;
        mergeSort(data, sisikiri, titiktengah, sortir, ascending);
        mergeSort(data, titiktengah + 1, sisikanan, sortir, ascending);
        merge(data, sisikiri, titiktengah, sisikanan, sortir, ascending);
    }
}

void Sorting(const string& usernameadmin){
    ifstream file("data_barang.json");
    json data;
    if(!file.is_open()) {
        cout << "Gagal membuka data_barang.json" << endl;
        return;
    }
    file >> data;

    int sortChoice;
    cout << "\n========== Pilihan Sorting ==========\n";
    cout << "|| 1. Sort berdasarkan Nama Barang (A-Z)\n";
    cout << "|| 2. Sort berdasarkan Nama Barang (Z-A)\n";
    cout << "|| 3. Sort berdasarkan Harga (Termurah)\n";
    cout << "|| 4. Sort berdasarkan Harga (Termahal)\n";
    cout << "|| 5. Sort berdasarkan Stok (Tersedikit)\n";
    cout << "|| 6. Sort berdasarkan Stok (Terbanyak)\n" << endl;
    cout << "Masukkan pilihan sorting: ";
    cin >> sortChoice;

    string sortir;
    bool ascending;
    switch(sortChoice) {
        case 1: sortir = "nama"; ascending = true; break;
        case 2: sortir = "nama"; ascending = false; break;
        case 3: sortir = "harga"; ascending = true; break;
        case 4: sortir = "harga"; ascending = false; break;
        case 5: sortir = "stok"; ascending = true; break;
        case 6: sortir = "stok"; ascending = false; break;
        default: 
            cout << "Pilihan tidak valid, menggunakan default sort (nama A-Z)\n";
            sortir = "nama"; ascending = true;
    }

    if (data["barang"].size() > 0) {
        mergeSort(data, 0, data["barang"].size() - 1, sortir, ascending);
    }
    ofstream outFile("data_barang.json");
    outFile << data.dump(4);
    outFile.close();
    cout << "Data Toko" << endl;
    for (const auto& barang : data["barang"]) {
        cout << "Nama: " << barang["nama"] << " || Harga: " << barang["harga"] << " || Stok: " << barang["stok"] << endl;
    }
}

void Carikan(string usernameadmin){
    ifstream file("data_barang.json");
    json data;
    if (!file.is_open()) {
        cout << "Gagal membuka data_barang.json" << endl;
        return;
    }
    file >> data;

    do{

        int pilihan;
        cout << "\n== Menu Pencarian ==" << endl;
        cout << "1. Cari berdasarkan Nama" << endl;
        cout << "2. Cari berdasarkan Harga" << endl;
        cout << "3. Kembali ke Kelola Barang" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();
        
        string cari_nama;
        int cari_harga;
        bool Cari = false;
        
        if (pilihan == 1) {
            cout << "Masukkan nama barang yang dicari: ";
            getline(cin, cari_nama);
            Cari = true;
        } 
        else if (pilihan == 2) {
            cout << "Masukkan harga yang dicari: ";
            cin >> cari_harga;
            cin.ignore();
            Cari = true;
        }
        else if (pilihan == 3) {
            return;
        }
        else {
            cout << "Pilihan tidak valid" << endl;
            pilihan = 3;
        }
        
        bool found = false;
        cout << "\n== Data Toko " << usernameadmin << " ==" << endl;
        
        // Mulai Linear Search
        for (const auto& barang : data["barang"]) {
            if (barang["Toko"] == usernameadmin) {
                // Skip jika sedang mode search dan tidak match
                if (Cari) {
                    if (pilihan == 1) {
                        // Cari nama 
                        string nama_barang = barang["nama"].get<string>();
                        string cari_lower = cari_nama;
                        transform(nama_barang.begin(), nama_barang.end(), nama_barang.begin(), ::tolower);
                        transform(cari_lower.begin(), cari_lower.end(), cari_lower.begin(), ::tolower);
                        
                        if (nama_barang.find(cari_lower) == string::npos) {
                            continue;
                        }
                    } 
                    else if (pilihan == 2) {
                        // Cari harga exact match
                        if (barang["harga"].get<int>() != cari_harga) {
                            continue;
                        }
                    }
                }
                
                // Tampilkan barang yang match
                cout << "Nama: " << barang["nama"] << endl;
                cout << "Stok: " << barang["stok"] << endl;
                cout << "Berat: " << barang["berat"] << endl;
                cout << "Harga: " << barang["harga"] << endl;
                cout << "------------------------" << endl;
                found = true;
            }
        }
        if (!found) {
            if (Cari) {
                cout << "Tidak ditemukan barang yang sesuai dengan kriteria pencarian!" << endl;
            } else {
                cout << "Tidak Ada Barang" << endl;
            }
        }
        }while(pilihan != 3);
}


// ----------------------------------------
// |                                      |
// |          FITUR ADMIN                 |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

void lihatbarang(const string& usernameadmin, bool show_only){ // MELIHAT BARANG PEMILIK TOKO ITU SENDIRI
    ifstream file("data_barang.json");
    json data;
    if(!file.is_open()) {
        cout << "Gagal membuka data_barang.json" << endl;
        return;
    }
    file >> data;
    bool exit_menu = false;
    while (!exit_menu) {

        bool found = false;
        cout << "\n== Data Toko " << usernameadmin << " ==" << endl;
        for (const auto& barang : data["barang"]) {
            if (barang["Toko"] == usernameadmin) { 
                cout << "|| Nama: " << barang["nama"];
                cout << "  | Stok: " << barang["stok"] << endl;
                cout << "|| Berat: " << barang["berat"];
                cout << "  | Harga: " << barang["harga"] << endl << endl;
                found = true;
            }
        }
        if (!found){
            cout << "Tidak Ada Barang" << endl;
        }
        if (show_only == false){
            int pilihan=0;
            cout << "||================= Menu =================||" << endl;
            cout << "||  [1] Urutkan || [2] Cari || [3] Keluar ||" << endl;
            cout << "||========================================||" << endl << endl;
            cin >> pilihan;
            cin.ignore();
            switch(pilihan){
                case 1:
                    Sorting(usernameadmin);
                    break;
                case 2:
                    Carikan(usernameadmin);
                    break;
                case 3:
                    pilihan = 0;
                    return;
                default:
                    cout << "Pilihan Tidak Valid" << endl;
            }
        }
        else {
            return;
        }
    }
}

void lihatbarangpembeli(const string namaToko) { // MELIHAT SEMUA BARANG YANG DIJUAL DARI BERBAGAI TOKO
    ifstream file("data_barang.json");
    json data;
    if(!file.is_open()) {
        cout << "Gagal membuka data_barang.json" << endl;
        return;
    }
    file >> data;
        
    cout << "\n== Data barang penjual ==" << endl;
    for (const auto& barang : data["barang"]) {
        if (barang["stok"] > 0 && barang["Toko"] == namaToko){
            cout << "|| Toko: " << barang["Toko"] << endl;
            cout << "|| Nama: " << barang["nama"];
            cout << "  | Stok: " << barang["stok"] << endl;
            cout << "|| Berat: " << barang["berat"];
            cout << "  | Harga: " << barang["harga"] << endl << endl;
        }
    }
}
    

void barangpenjual(const string& usernameadmin, bool show_only){ // MENAMBAH BARANG (PENJUAL)
    InputBarang TambahBarang;
    cin.ignore();
    cout << "Masukkan Nama Barang: ";
    getline(cin, TambahBarang.namabarang);
    cout << "Masukkan Stok Barang: ";
    cin >> TambahBarang.stok;
    cout << "Masukkan Berat per Barang (Satuan kg): ";
    cin >> TambahBarang.berat;
    cout << "Masukkan Harga per 1kg Barang: ";
    cin >> TambahBarang.harga;

    json data;
    ifstream inFile("data_barang.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        data["barang"] = json::array();
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

    lihatbarang(usernameadmin,show_only);
}

void editbarang(string& cari_nama, const string& usernameadmin, bool show_only) { // MENGEDIT BARANG (PENJUAL)
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
    lihatbarang(usernameadmin,show_only);

    ketemu = false;
    cout << "\n=== Edit Data Barang ===" << endl;
    cout << "Masukkan nama barang yang ingin diubah: ";
    cin.ignore();
    getline (cin, cari_nama);

    for (auto& item : data["barang"]) {
        if (item["nama"] == cari_nama && item["Toko"] == usernameadmin) {
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
        cout << "Barang dengan nama \"" << cari_nama << "\" tidak ditemukan!!" << endl;
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

void menghapusbarang(string& cari_nama, bool& ketemu, const string& usernameadmin, bool show_only) {
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
    lihatbarang(usernameadmin,show_only);

    auto& list = data["barang"];
    ketemu = false;

    cout << "\n=== Hapus Data Barang ===" << endl;
    cout << "Masukkan nama barang yang ingin dihapus: ";
    cin.ignore();
    getline(cin, cari_nama);

    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i]["nama"] == cari_nama && list[i]["Toko"] == usernameadmin) {
            list.erase(list.begin() + i);
            ketemu = true;
            break;
                
        }
    }

    if (ketemu) {
        ofstream outFile("data_barang.json");
        if (outFile.is_open()) {
            outFile << data.dump(4);
            outFile.close();
            cout << "Data \"" << cari_nama << "\" sudah terhapus." << endl;
        } else {
            cout << "Gagal menyimpan perubahan." << endl;
        }
    } else {
        cout << "Data \"" << cari_nama << "\" tidak ditemukan." << endl;
    }
}

void tarikuang(const string usernameadmin, const string passwordadmin, int& dana) { 
    json dataUser, dataPembelian;
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

    // Kumpulkan penghasilan dari semua barang
    for (auto& barang : dataPembelian["pembelian"]) {
        if (barang["Status"] == "Selesai" && barang["Toko"] == usernameadmin) {
            int totalHarga = barang["total_harga"];
            totalTarik += totalHarga;
            barang["Status"] = "SelesaiDitarik";
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
            dana = seller["dana"];
            break;
        }
    }

    // Memasukkan Status SelesaiDitarik Ke Barang Yang Berstatus Selesai
    ofstream outFilebarang("data_pembelian.json");
    outFilebarang << dataPembelian.dump(4);
    outFilebarang.close();

    // Simpan ke file
    ofstream outFile("data_user.json");
    outFile << dataUser.dump(4);
    outFile.close();

    cout << "Saldo berhasil ditarik dari pembelian: Rp" << totalTarik << endl;
    cout << "Saldo total penjual sekarang: Rp" << dana << endl;
}


void laporanpenjualan(const string& usernameadmin) { // Laporan Penjualan Jika Status Selesai
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
        if (data["Status"] == "SelesaiDitarik" && data["Toko"] == usernameadmin) {
            cout << "Nama Pembeli: " << data["username"] << endl;
            cout << "Nama Barang: " << data["nama_barang"] << endl;
            cout << "Jumlah Barang: " << data["jumlah"] << endl;
            cout << "Total Harga: " << data["total_harga"] << endl;
            cout << "==================================" << endl;
        }
    }
    
    for (auto& data : dataPembelian["pembelian"])
    {
        if (data["Status"] == "SelesaiDitarik" && data["Toko"] == usernameadmin) {
            penjualan = data["total_harga"];
            totalpenjualan += penjualan;
        }
    }

    cout << "Total Penghasilan Adalah: " << totalpenjualan << endl;
}

void konfirmasiPesananAdmin(const string& usernameadmin) { // Mengubah Status Konfirmasi Dari Dikirim menjadi Selsai
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

void konfirmasiPesanan(const string& username) { // Mengkonfirmasi Pesanan Dari Dipesan Menjadi Dikirim Dan Menjadi Fungsi Rekursif Untuk Fungsi lihatpesanan()
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
        if (pesan["username"] == username && pesan["Status"] == "Dipesan") {
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
        if (pesan["UID"] == noPesanan && pesan["username"] == username) {
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

void lihatpesanan(const string& username) { // Melihat Pesanan Dan Ada Menu Tambahan
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
        if (pesan["username"] == username) {
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
                if (pesan["username"] == username) {
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




void topup(const string& username, const string& password){ // Menambahkan Dana Ke User (pembeli)
    json data;
    ifstream inFile("data_user.json");
    int fitur, pilihantopup, dana, jumlahtopup;

    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_user.json" << endl;
        return;
    }

    bool found = false;
    for (auto& user : data["pembeli"]) {
        if (user["username"] == username && user["password"] == password) {
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
                cin >> pilihantopup;
                switch (pilihantopup) {
                case 1:
                    jumlahtopup = 50000;
                    break;
                case 2:
                    jumlahtopup = 100000;
                    break;
                case 3:
                    jumlahtopup = 250000;
                    break;
                case 4:
                    jumlahtopup = 500000;
                    break;
                case 5:
                    jumlahtopup = 1000000;
                    break;
                case 6:
                    jumlahtopup = 1500000;
                    break;
                default:
                    cout << "Pilihan tidak valid" << endl;
                    continue;
                }
                if (user.contains("dana") && user["dana"].is_number()) {
                    user["dana"] = user["dana"].get<int>() + jumlahtopup;
                } else {
                    user["dana"] = jumlahtopup;
                }
                dana = user["dana"];
                found = true;
                break;
            }
            else if (fitur == 2) {
                cout << "Masukkan jumlah top up: ";
                cin >> jumlahtopup;
                if (user.contains("dana") && user["dana"].is_number()) {
                    user["dana"] = user["dana"].get<int>() + jumlahtopup;
                } else {
                    user["dana"] = jumlahtopup;
                }
                dana = user["dana"];
                found = true;
                break;
            }
            else if (fitur == 3) {
                found = true;
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
            if (fitur != 3) {
                cout << "Top up berhasil. Saldo sekarang: " << dana << endl;
            }
        } else {
            cout << "Gagal menyimpan ke file data_user.json" << endl;
        }
    } else {
        cout << "Data pengguna tidak ditemukan. Top up gagal." << endl;
    }
}  

void beli(const string& username, const string& password) { // User Membeli Barang Dan Status Barang Dipesan
    json dataBarang, dataUser, dataPembelian;
    string namaBarang, namaToko;
    int jumlahBeli, dana;
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
    bool userFound = false;
    for (auto& user : dataUser["pembeli"]) {
        if (user["username"] == username && user["password"] == password) {
            dana = user["dana"];
            userFound = true;
            break;
        }
    }
    if (!userFound) {
        cout << "User not found or invalid credentials." << endl;
        return;
    }
    
        cout << "===== LIST TOKO =====" << endl;
    for (const auto& user : dataUser["penjual"])
    {
        cout << user["username"] << endl;
    }
    cout << "\nMasukkan nama toko yang ingin dikunjungi: ";
    cin.ignore();
    getline(cin, namaToko);

    // Tampilkan daftar barang
    lihatbarangpembeli(namaToko);



    int maxUID = 0;
    for (const auto& pesan : dataPembelian["pembelian"]) {
        int currentUID = pesan["UID"];
        if (currentUID > maxUID) {
            maxUID = currentUID;
        }
    }

    cout << "\nMasukkan nama barang yang ingin dibeli: ";
    getline(cin, namaBarang);
    for (auto& barang : dataBarang["barang"]) {
        if (barang["nama"] == namaBarang && barang["Toko"] == namaToko) {
            found = true;
            cout << "Masukkan jumlah yang ingin dibeli: ";
            cin >> jumlahBeli;

            float totalHarga = jumlahBeli * (int(barang["harga"]) * float(barang["berat"]));
            cout << "Total harga : " << totalHarga << endl;
            cout << "Dana Anda : " << dana << endl;

            // Validasi
            if (jumlahBeli > int(barang["stok"])) {
                cout << "Stok tidak cukup!\n";
                return;
            }

            if (dana < totalHarga) {
                cout << "Saldo tidak cukup!\n";
                return;
            }

            // Kurangi stok dan saldo
            barang["stok"] = int(barang["stok"]) - jumlahBeli;
            dana -= totalHarga;

            // Update saldo pembeli di file
            for (auto& user : dataUser["pembeli"]) {
                if (user["username"] == username && user["password"] == password) {
                    user["dana"] = dana;
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
                {"username", username},
                {"nama_barang", namaBarang},
                {"jumlah", jumlahBeli},
                {"total_harga", totalHarga},
                {"Status", "Dipesan"}
            });

            cout << "Pembelian berhasil! Total harga: " << totalHarga << ", Sisa saldo: " << dana << endl;
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

string editdetail(string username, string password) { // Mengedit Detail Pembeli
    ifstream inFile("data_user.json");
    json data;
    bool verif = false, verif2 = false;
    int verifAttempt = 0;
    string passwordulang;

    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    } else {
        cout << "Gagal membuka file data_user.json" << endl;
        return password;
    }

    while (verifAttempt < 3) {
        cout << "\n=== Edit Detail User ===" << endl;
        cout << "Masukkan Password Anda : ";
        cin >> passwordulang;

        // Cari pengguna berdasarkan username dan password
        for (auto& detail : data["pembeli"]) {
            if (detail["username"] == username && detail["password"] == passwordulang) {
                verif = true;
                verif2 = true;
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
                    return password_baru;
                    } 
                else {
                    cout << "Gagal menyimpan perubahan ke file data_user.json" << endl;
                    }
                return password;
            }
        }

        // Loop Attempt
        while (!verif && verifAttempt < 3 && !verif2) {
            verifAttempt++;
            cout << "Password salah, silahkan ulangi kembali: ";
            cin >> passwordulang;

            for (auto& detail : data["pembeli"]) {
                if (detail["username"] == username && detail["password"] == passwordulang) {
                    verif2 = true;
                    break; // Break Loop
                }
            }

            if (!verif && verifAttempt < 3 && !verif2) {
                cout << "Percobaan " << verifAttempt << " dari 3 gagal." << endl;
        }
            }
    }

    if (!verif) {
        cout << "Terlalu banyak percobaan, kembali ke menu pembeli. . ." << endl;
    }
    return password;
}

void hapuspesanan(const string& username, const string& password){ // Menghapus Pesanan Yang Telah Dipesan
    ifstream inFileP("data_pembelian.json");
    ifstream inFileU("data_user.json");
    json dataP, dataU;
    int pilihan;
    if (inFileP.is_open()){
        inFileP >> dataP;
        inFileP.close();
    } else {
        cout << "Gagal membuka file data_pembelian.json" << endl;
        return;
    }
    if (inFileU.is_open()){
        inFileU >> dataU;
        inFileU.close();
    } else {
        cout << "Gagal membuka file data_user.json" << endl;
        return;
    }

    cout << "\n== Data Pesanan ==" << endl;
    for (auto& pesan : dataP["pembelian"]) {
        if (pesan["username"] != username) {
            continue;
        }
        cout << "No. Pesanan: " << pesan["UID"] << endl;
        cout << "nama: " << pesan["nama_barang"] << endl;
        cout << "jumlah  : " << pesan["jumlah"] << endl;
        cout << "harga: " << pesan["total_harga"] << endl;
        cout << "status: " << pesan["Status"] << endl;
        cout << endl;
    }
    cout << "[Masukkan 0 untuk kembali]" << endl;
    cout << "Pilih No. Pesanan yang ingin dihapus : ";
    cin >> pilihan;

    if (pilihan == 0) {
        return;
    }

    auto& pesanan = dataP["pembelian"];
    bool terhapus = false;
    for (auto it = pesanan.begin(); it != pesanan.end(); ) {
        if ((*it)["UID"] == pilihan && (*it)["username"] == username) {
            if ((*it)["Status"] == "Dipesan" || (*it)["Status"] == "SelesaiDitarik") {
                it = pesanan.erase(it);
                cout << "Pesanan dengan No. Pesanan " << pilihan << " telah dihapus" << endl;
                terhapus = true;
                break;
            } else {
                cout << "Pesanan Sudah Di Konfirmasi Dan Tidak Bisa Dibatalkan!!" << endl;
                terhapus = true;
                break;
            }
        }
        else {
            ++it;
        }
    }
    if (pilihan != 0 && !terhapus){
        cout << "Nomor Pesanan Tidak Sesuai" << endl;
        return;
    }
    ofstream outFileP("data_pembelian.json");
    outFileP << dataP.dump(4);
    outFileP.close();
}

int kelolaBarang(string usernameadmin, string passwordadmin){
    do {
        int pilihan;
        bool show_only = true;
        cout << "\n=== Kelola Barang ===" << endl;
        cout << "|| 1. Tambah Barang" << endl;
        cout << "|| 2. Lihat Barang" << endl;
        cout << "|| 3. Edit Barang" << endl;
        cout << "|| 4. Hapus Barang" << endl;
        cout << "|| 5. Kembali ke Menu Penjual" << endl;
        cout << "|| Masukkan Pilihan : " << endl;
        cin >> pilihan;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            bebersih();
            cout << "Input tidak valid. Masukkan angka." << endl;
            continue;
        }
        
        switch (pilihan) {
            case 1:
                bebersih();
                barangpenjual(usernameadmin, show_only);
                break;
            case 2:
                bebersih();
                show_only = false;
                lihatbarang(usernameadmin,show_only);
                break;
            case 3:
                bebersih();
                editbarang(cari_nama, usernameadmin, show_only);
                break;
            case 4:
                bebersih();
                menghapusbarang(cari_nama, ketemu, usernameadmin, show_only);
                break;
            case 5:
                bebersih();
                cout << "Kembali ke Menu Penjual" << endl;
                return true;
            default:
                bebersih();
                cout << "Pilihan tidak valid" << endl;
        }
    }while(true);
}

// ----------------------------------------
// |                                      |
// |      REGIS, LOGIN DAN MENU           |
// |            GACOR LE                  |
// |                                      |
// |--------------------------------------|

void menuadmin(const string usernameadmin, const string passwordadmin) {

    while (pilihan != 5)
    {
        int pilihan;
        cout << "\n=== Menu Penjual / Admin ===" << endl;
        cout << "1. Kelola Barang" << endl;
        cout << "2. Tarik Penghasilan" << endl;
        cout << "3. Laporan Penjualan" << endl;
        cout << "4. Cek Pesanan Pembeli" << endl;
        cout << "5. Kembali" << endl;
        cout << "Masukkan Pilihan: ";
        cin >> pilihan;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Input tidak valid. Masukkan angka." << endl;
            continue;
        }

        switch (pilihan) {
        case 1:
            bebersih();
            kelolaBarang(usernameadmin,passwordadmin);
            pilihan = 0;
            break;
        case 2:
            bebersih();
            tarikuang(usernameadmin, passwordadmin, dana);
            pilihan = 0;
            break;
        case 3:
            bebersih();
            laporanpenjualan(usernameadmin);
            pilihan = 0;
            break;
        case 4:
            bebersih();
            konfirmasiPesananAdmin(usernameadmin);
            pilihan = 0;
            break;
        case 5:
            bebersih();
            cout << "Kembali Ke Menu Login" << endl;
            pilihan = 0;
            return;
        default:
            bebersih();
            cout << "pilihan tidak valid" << endl;
            pilihan = 0;
        }
    }
}

void menupembeli(const string username, string password) {
    do {
        cout << "\n=== Menu Pembeli / User ===" << endl;
        cout << "1. Top up dana" << endl;
        cout << "2. Beli barang" << endl;
        cout << "3. Ubah Detail" << endl;
        cout << "4. Hapus Pesanan" << endl;
        cout << "5. Lihat Pesanan" << endl;
        cout << "6. Kembali" << endl;
        cout << "Masukkan Pilihan: ";
        cin >> pilihan;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            bebersih();
            cout << "Input tidak valid. Masukkan angka." << endl;
            continue;
        }

        switch (pilihan) {
        case 1:
            bebersih();
            topup(username, password);
            continue;
        case 2:
            bebersih();
            beli(username, password);
            continue;
        case 3:
            bebersih();
            password = editdetail(username, password);
            continue;
        case 4:
            bebersih();
            hapuspesanan(username, password);
            continue;
        case 5:
            bebersih();
            lihatpesanan(username);
            continue;
        case 6:
            bebersih();
            cout << "Kembali Ke Menu Login" << endl;
            return;
        default:
            bebersih();
            cout << "pilihan tidak valid" << endl;
        }
    } while (pilihan != 6);
}

bool isAllDigits(const string& str) {
    for (char c : str) {
        if (c < '0' || c > '9') { // Cek apakah karakter bukan digit
            return false;
        }
    }
    return true;
}

void regis(string& username, string& password, string* penjual, string* pwsell, 
           int& jumlah_user, int& pilihan, string& nohp, string& hppj, int& dana) {
    bool repeat = false;
    cout << "\n=== Menu Pendaftaran ===" << endl;
    cout << "1. Daftar sebagai penjual" << endl;
    cout << "2. Daftar sebagai pembeli" << endl;
    cout << "3. Kembali" << endl;
    cout << "Pilihan Anda: ";
    cin >> pilihan;
    cin.ignore();
    
    switch (pilihan) {
    case 1:
        if (repeat == false) {
            bebersih();
        }
        cout << "Masukkan username: ";
        cin >> penjual[jumlah_user];
        if (cekuser()) {  // Asumsi cekuser() sudah didefinisikan
            repeat = true;
            bebersih();
            cout << "Username sudah terdaftar, silahkan cari username lain" << endl;
            return;
        }
        cout << "Masukkan password: ";
        cin >> pwsell[jumlah_user];
        
        // Validasi nomor HP untuk penjual
        while (true) {
            cout << "Masukkan No. HP (hanya angka): ";
            cin >> hppj;
            if (isAllDigits(hppj)) break;
            cout << "ERROR: Nomor HP hanya boleh berisi angka!" << endl;
        }
        
        cin.ignore();
        datapenjual(penjual, pwsell, jumlah_user, hppj, dana);  // Asumsi fungsi ini sudah didefinisikan
        jumlah_user++;
        break;
    
    case 2:
        if (repeat == false) {
            bebersih();
        }
        cout << "Masukkan username: ";
        cin >> username;
        if (cekuser()) {  // Asumsi cekuser() sudah didefinisikan
            repeat = true;
            bebersih();
            cout << "Username sudah terdaftar, silahkan cari username lain" << endl;
            return;
        }
        cout << "Masukkan password: ";
        cin >> password;
        
        // Validasi nomor HP untuk pembeli
        while (true) {
            cout << "Masukkan No. HP (hanya angka): ";
            cin >> nohp;
            if (isAllDigits(nohp)) break;
            cout << "ERROR: Nomor HP hanya boleh berisi angka!" << endl;
        }
        
        cin.ignore();
        datapembeli(username, password, nohp);  // Asumsi fungsi ini sudah didefinisikan
        break;

    case 3:
        cout << "Kembali Ke Menu Sebelumnya" << endl;
        menuutama();  // Asumsi fungsi ini sudah didefinisikan
        break;
    default:
        cout << "Pilihan tidak valid." << endl;
        break;
    }
}

void login(string& username, string& password, string* penjual, string* pwsell, int* jumlah_user, int& pilihan, int& percobaan, string& nohp, string& hppj) {
    json data;
    std::ifstream inFile("data_user.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    cout << "\n=== Menu Login ===" << endl;
    cout << "1. Login sebagai penjual" << endl;
    cout << "2. Login sebagai pembeli" << endl;
    cout << "3. Kembali" << endl;
    cout << "Pilihan anda: ";
    cin >> pilihan;

    if (cin.fail()) {
        cin.clear();
        cin.ignore();
        cout << "Input tidak valid. Masukkan angka." << endl;
        return login(username, password, penjual, pwsell, jumlah_user, pilihan, percobaan, nohp, hppj);
    }

    switch (pilihan) {
    case 1:
        cout << "Masukkan username: ";
        cin >> penjual[*jumlah_user];
        cout << "Masukkan password: ";
        cin >> pwsell[*jumlah_user];
        if (verifikasiPenjual(data, penjual[*jumlah_user], pwsell[*jumlah_user])) {
            bebersih();
            cout << "Berhasil Login ke Menu Penjual!" << endl;
            menuadmin(penjual[*jumlah_user], pwsell[*jumlah_user]);
        } 
        else {
            cout << "login gagal" << endl;
            cout << "username atau password salah" << endl;
            return;
        }
        break;

    case 2:
        cout << "Masukkan username: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;
        cin.ignore();
        if (verifikasiPembeli(data, username, password)) {
            bebersih();
            cout << "login berhasil" << endl;
            menupembeli(username, password);
        } 
        else {
            bebersih();
            cout << "login gagal" << endl;
            cout << "username atau password salah" << endl;
            return;
        }
        break;

    case 3:
        bebersih();
        cout << "Kembali Ke Menu Sebelumnya" << endl;
        break;

    default:
        bebersih();
        cout << "Pilihan tidak valid." << endl;
        break;
    }
    return;
}



void menuutama() {
    do {
        try {
            cout << "\n=== Menu Utama ===" << endl;
            cout << "1. Daftar Pengguna" << endl;
            cout << "2. Login" << endl;
            cout << "3. Keluar" << endl;
            cout << "Pilihan Anda: ";
            
            cin >> pilihan;
            
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(); 
                throw invalid_argument("Input harus berupa angka.");
            }

            switch (pilihan) {
                case 1:
                    regis(username, password, penjual, pwsell, jumlah_user, pilihan, nohp, hppj, dana);
                    pilihan = 0;
                    break;
                case 2:
                    login(username, password, penjual, pwsell, &jumlah_user, pilihan, percobaan, nohp, hppj);
                    pilihan = 0;
                    break;
                case 3:
                    cout << "Terimakasih Sudah Menggunakan Program Kami" << endl;
                    return;
                default:
                    bebersih();
                    cout << "Pilihan tidak valid. Masukkan angka 1 - 3." << endl;
                    menuutama();
            }

        } catch (const invalid_argument& e) {
            cout << "Terjadi kesalahan: " << e.what() << endl;
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

