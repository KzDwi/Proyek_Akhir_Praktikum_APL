📦 Sistem Manajemen Toko Online 🛒
Sebuah program C++ yang memungkinkan pengguna untuk berperan sebagai penjual atau pembeli dalam sebuah platform toko online. Program ini dilengkapi dengan fitur-fitur lengkap seperti manajemen barang, transaksi, laporan penjualan, dan banyak lagi!

🧑‍💻 Kelompok Rantasik

Anggota:
    Elfin Sinaga         - 2409106024
    Dwi Prasetyawan      - 2409106028
    Ahnaf Aliyyu         - 2409106035
    Rangga Aditya Rahman - 2409106044

🌟 Fitur Utama

🛍️ Fitur Pembeli:
    Top Up Dana: Tambah saldo untuk belanja.
    Beli Barang: Pesan barang dari berbagai toko.
    Lihat Pesanan: Lacak status pesanan (Dipesan/Dikirim/Selesai).
    Hapus Pesanan: Batalkan pesanan yang belum dikonfirmasi.
    Ubah Detail Akun: Ganti password atau nomor HP.

🏪 Fitur Penjual:
    Kelola Barang: Tambah, edit, hapus, dan lihat stok barang.
    Tarik Penghasilan: Konversi pendapatan menjadi saldo penjual.
    Laporan Penjualan: Lihat riwayat transaksi dan total penghasilan.
    Konfirmasi Pesanan: Ubah status pesanan dari Dikirim ke Selesai.

🔍 Fitur Pencarian & Sorting:
    Cari Barang: Berdasarkan nama atau harga.
    Sorting Barang: Urutkan berdasarkan nama (A-Z/Z-A), harga (termahal/termurah), atau stok (terbanyak/tersedikit).

🛠️ Teknologi & Konsep
    Bahasa: C++ (menggunakan Standard Library dan nlohmann/json untuk manipulasi file JSON).
    Struktur Data: Array, Struct, dan Merge Sort untuk pengurutan data.
    Penyimpanan: Data disimpan dalam file JSON (data_user.json, data_barang.json, data_pembelian.json).

    Fitur Lanjutan:
        Validasi input (nomor HP hanya angka).
        Rekursif untuk fungsi konfirmasi pesanan.
        Modular programming dengan pemisahan fungsi-fungsi khusus.

📂 Struktur File
    data_user.json: Menyimpan data pengguna (pembeli & penjual).
    data_barang.json: Menyimpan data barang dari semua toko.
    data_pembelian.json: Menyimpan riwayat transaksi dan status pesanan.

📝 Contoh Penggunaan
    Daftar sebagai Penjual:
        Tambahkan barang seperti "Kursi" dengan harga Rp55.000 dan stok 24.
    Daftar sebagai Pembeli:
        Top up saldo, lalu beli barang dari toko yang tersedia.
    Penjual:
        Lihat laporan penjualan dan tarik penghasilan ke saldo.
