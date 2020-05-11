
//Required SQL Lib Here
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

//Include STL LIB Here
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

//User-made header
#include "SQLcon.h"
#include "FILEScon.h"

// Login or User Class
class Login
{
public:
	int uid = 0, urole = 0;
	std::string uname;

	bool status = FALSE;
	void login(sqlcon sqlcn);
	void signout();
	void regis(sqlcon sqlcn);
	

private:
	bool confirm(char y);
};
bool Login::confirm(char y) {
	if (y == 'Y' or y == 'y')
		return TRUE;
	else
		return FALSE;
}
void Login::login(sqlcon sqlcn)
{
	std::string u;
	std::string p;
	int i = 0;


	std::cout << "Please login to continue" << std::endl
		<< "------------------------------" << std::endl;
	do {
		++i;
		std::cout << "Username : ";
		std::cin >> u;
		std::cout << "Password : ";

		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode = 0;
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

		std::cin >> p;

		SetConsoleMode(hStdin, mode);

		std::cout << std::endl;


		sqlcn.exec("DECLARE @responseMessage NVARCHAR(250);exec dbo.LoginUser \
					@pUsername='" + u + "', \
					@pPassword='" + p + "',\
					@pResponseMessage=@responseMessage OUTPUT;\
					select @responseMessage;");


		//std::cout << "Running Query : " << std::endl;
		//FetchSQL
		char result[250];
		std::string rs;
		while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_C_DEFAULT, &result, sizeof(result), NULL);
		}

		//free statement
		sqlcn.freestmt();

		//change to string
		rs = result;
		if (rs != "Username or Password is incorrect") {
			// Check if output is what program desire
			try
			{
				std::stoi(rs);
			}
			catch (const std::exception&)
			{
				std::cout << "Something go wrong" << std::endl;
				this->status = FALSE;
				return;
			}

			// Get Login Detail
			sqlcn.exec("\
					select u.id, u.nama, r.id from \
					dbo.[tbl_user] u Join dbo.[role_user] r \
					ON u.id_role_user = r.id and u.id ="+rs+"; ");
			//Fetch Data
			int userid = 0;
			char username[50];
			int userrole = 0;
			while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
				SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &userid, 0, NULL);
				SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_C_DEFAULT, &username, sizeof(username), NULL);
				SQLGetData(sqlcn.SQLStatementHandle, 3, SQL_INTEGER, &userrole, 0, NULL);
			}

			sqlcn.freestmt();

			// Store to class variable
			this->uid = userid;
			this->uname = username;
			this->urole = userrole;

			std::cin.ignore();
			std::cout << "Login success " <<std::endl
				<< "Welcome, " << this->uname;
			std::cout <<std::endl << "------------------------------" <<std::endl;
			this->status = TRUE;
			return;
		}
		else {
			std::cout << "Can't login : " << result << std::endl;
		}
	} while (this->status == FALSE and i < 3);
	std::cin.ignore();
	this->status = FALSE;
}
void Login::signout() {
	this->status = FALSE;
	this->uid = 0;
	this->urole = 0;
}
void Login::regis(sqlcon sqlcn) {
	char conf;
	std::cout << "------- \n" << "Do you want to add new user [y/n] ? : ";
	std::cin >> conf;

	//check confirmation
	if (this->confirm(conf)) {
		//check user privilage
		if (this->urole > 2)
			return;

		//set variable
		std::string uu;
		std::string up;
		std::string una;
		int  r = 3;

		//input variable
		std::cout << "> Masukan nama  : ";
		std::cin.ignore();
		std::getline(std::cin, una);
		std::cout << "> Masukan username  : " ;
		std::cin >> uu;
		std::cout << "> Masukan password : " ;
		std::cin >> up;
		std::cout << "> Masukan role [1,2,3] (3) : " ;
		std::cin >> r;
		//store to DB
		sqlcn.exec("\
			DECLARE @responseMessage NVARCHAR(250);\
			EXEC dbo.SignUpUser\
				@pUsername = '"+uu+"',\
				@pName	   = '"+una+"',\
				@pPassword = '"+up+"',\
				@pRole = "+ std::to_string(r)+",\
				@pResponseMessage = @responseMessage OUTPUT;\
			SELECT @responseMessage;\
		");

		char result[250];
		std::string rs;
		while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_C_DEFAULT, &result, sizeof(result), NULL);
		}

		sqlcn.freestmt();

		rs = result;
		if (rs == "SUCCES") {
			std::cout << "Berhasil Menambah User" << std::endl;
			return;
		}
		std::cout << "Gagal Menambah User" << std::endl;
		return;
	}


	return;

}


// -----------------------------------------------
// FOCUS HERE 2
// ALL main command goes here
// All function has bool type
// Return False only, True value for terminate Program
// -----------------------------------------------
bool confirm(std::string msg = "Apakah Anda ingin menambahkan lagi [Y/N] :") {
	char c;
	std::cout << msg;
	std::cin >> c;
	std::cin.ignore();
	if (c == 'y' or c == 'Y')
		return 1;
	return 0;
}

bool Test() {
	std::cout << "Fungsi perintah jalan di fungsi ini" << std::endl;
	return FALSE;
}

// Role Base Access Control
// let's implement it on c++ not store in sql
bool RBAC(Login lgn, std::vector<int> can_access_list) {
	for (int i : can_access_list)
		if (i == lgn.urole)
			return 1;
	return 0;
}

// Menu kasir
bool Kasir(sqlcon sqlcn) {
	int menu = 0;
	int quantity = 1;
	bool lanjut0 = 1, lanjut1 = 1;
	
	std::vector<std::vector<std::string>> data1, data2;
	char rs1[255];
	std::string rss1;
	int r1, r2,r3,r4,r5;
	int i = 0;
	int id_ord =  0;
	int total_harga = 0;
	do {

		// Get Menu Data form database
		sqlcn.exec("SELECT id,nama,harga FROM [dbo].barang where is_valid=1");
		while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
			data1.push_back(std::vector<std::string>());
			SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &r1, 0, NULL);
			SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
			SQLGetData(sqlcn.SQLStatementHandle, 3, SQL_INTEGER, &r2, 0, NULL);

			data1[i].push_back(std::to_string(r1));
			data1[i].push_back(rs1);
			data1[i].push_back(std::to_string(r2));
			i++;
		}

		sqlcn.freestmt();

		if (i == 0) {
			std::cout << "Tidak dapat mendapatkan menu" << std::endl;
			return 0;
		}

		//ADD NEW ORDER TO TABLE Order
		sqlcn.exec("INSERT INTO [dbo].[order]([id_kasir],[tgl_pembelian],[last_edit], [is_valid])\
				OUTPUT Inserted.ID\
				VALUES(1, GETDATE(), GETDATE(), 0)\
				");

		while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &id_ord, 0, NULL);
		}

		sqlcn.freestmt();

		if (id_ord == 0) {
			std::cout << "Tidak dapat mengorder" << std::endl;
			return 0;
		}


		// looping until all menu are inputed
		do {

				
			std::cout << "---------MenuFastFood---------" << std::endl;
			std::cout << "===================================" << std::endl;
			std::cout << std::setw(3) << "No |" << std::setw(20) << "Produk |"
					  <<std::setw(12) << "Harga|" << std::endl;
			std::cout << " ==================================" << std::endl;
			for (auto a : data1) {
				std::cout << std::setw(3) << a[0] << std::setw(20) << a[1]
						<< std::setw(12) << a[2] << std::endl;
			}
			// sampai sini

			std::cout << std::endl;
			std::cout << "No pesanan     : ";
			std::cin >> menu;
			std::cout << "Banyak pesanan : ";
			std::cin >> quantity;


			// Input Data to Table detail_order
			sqlcn.exec("DECLARE @pResponseMessage nvarchar(255);\
				EXECUTE [dbo].[InputDetailOrder] \
				@pIdOrder = "+std::to_string(id_ord)+" \
				, @pIdBarang = "+ std::to_string(menu) +" \
				, @pKuantitas = " + std::to_string(quantity) + "\
				, @pResponseMessage = @pResponseMessage OUTPUT\
				SELECT @pResponseMessage");

			while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
				SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
			}
			sqlcn.freestmt();
			rss1 = rs1;

			if (rss1 != "SUCCES") {
				std::cout << rss1 <<" | Tidak dapat mendapatkan menu " << std::endl;
				return 0;
			}

						// Looping to add new menu
			lanjut0 = confirm();

		} while (lanjut0);
		
		// Set order to valid
		sqlcn.exec("UPDATE [dbo].[order]\
			SET is_valid = 1 \
			WHERE id = " + std::to_string(id_ord) + "; \
		");

		sqlcn.freestmt();

		// Get shopping chart list  via sql function
		i = 0;
		sqlcn.exec("SELECT * FROM [dbo].[func_get_order] (" + std::to_string(id_ord) + ")");
		while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
			data2.push_back(std::vector<std::string>());
			SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &r1, 0, NULL);
			SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_INTEGER, &r2, 0, NULL);
			SQLGetData(sqlcn.SQLStatementHandle, 3, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
			SQLGetData(sqlcn.SQLStatementHandle, 4, SQL_INTEGER, &r3, 0, NULL);
			SQLGetData(sqlcn.SQLStatementHandle, 5, SQL_INTEGER, &r4, 0, NULL);
			SQLGetData(sqlcn.SQLStatementHandle, 6, SQL_INTEGER, &r5, 0, NULL);

			data2[i].push_back(std::to_string(r1));
			data2[i].push_back(std::to_string(r2));
			data2[i].push_back(rs1);
			data2[i].push_back(std::to_string(r3));
			data2[i].push_back(std::to_string(r4));
			data2[i].push_back(std::to_string(r5));
			i++;
		}

		sqlcn.freestmt();

		// Get Total Harga
		sqlcn.exec("SELECT [dbo].[TotalHarga] (" + std::to_string(id_ord) + ")");
		while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
			SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &total_harga, 0, NULL);
		}
		sqlcn.freestmt();

		// Print Invoice
		std::cout << "======================================" << std::endl;
		std::cout << "---------------- Bill ----------------" << std::endl;
		std::cout << "======================================" << std::endl;
		for (auto a : data2) {
			std::cout   << std::setw(15) << std::left << a[2] << std::right
						<< std::setw(6) << a[3]
						<< std::setw(5) << a[4] 
						<< std::setw(11) << a[5] << std::endl;
		}
		std::cout << "--------------------------------------" << std::endl;
		std::cout	<< std::setw(11) << "Total Harga"  
					<< std::setw(26) << total_harga << std::endl;
		std::cout << "======================================" << std::endl;

		// Perhitungan Lain


		lanjut1 = confirm("Apakah Anda ingin tetap di menu kasir? [Y/N] : ");
	} while (lanjut1);
	std::cout << "----------" << std::endl;

	return 0;
}

// Menu Manager
bool Manager(sqlcon sqlcn, Login lgn) {
	std::string mn;
	char rs1[255], rs2[255];
	int r1 = NULL, r2 = NULL, r3 = NULL;
	int menu = 0;
	int quantity = 1;
	bool lanjut0 = 1, lanjut1 = 1;
	int i = 0;

	std::vector<std::vector<std::string>> data1;

	do {
		std::cout << std::endl << "Fungsi Yang Tersedia : " << std::endl;
		std::cout << "Order " << std::endl;
		std::cout << "1. List History Order " << std::endl;
		std::cout << "2. Export History Order " << std::endl;
		std::cout << "Menu " << std::endl;
		std::cout << "3. Tambah/Edit Menu  " << std::endl;
		std::cout << "4. List Menu  " << std::endl;
		std::cout << "5. List Jenis Menu  " << std::endl;
		std::cout << "6. Export Menu " << std::endl;
		std::cout << "Keuntungan" << std::endl;
		std::cout << "7. Laporan Keuntungan" << std::endl;
		std::cout << "8. Export Laporan Keuntungan" << std::endl;
		std::cout << "Pilih Fungsi : ";
		std::cin >> menu;

		switch (menu)
		{
			case (1): {
			sqlcn.exec("SELECT o.id, max(u.nama), sum(total_harga), convert(varchar, max(o.tgl_pembelian), 100)  \
					FROM[order] o, [detail_order] d, [tbl_user] u \
					WHERE o.id = d.id_order and o.id_kasir = u.id \
					GROUP BY(o.id) \
					");
			std::cout << "----- List History -----" << std::endl;

			std::cout << std::setw(3) << "ID" << std::setw(20) << "Kasir" 
					  << std::setw(20) << "Total Transaksi"
					  << std::setw(25) << "Waktu Transaksi" << std::endl;

			while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
				SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &r1, 0, NULL);
				SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
				SQLGetData(sqlcn.SQLStatementHandle, 3, SQL_INTEGER, &r2, 0, NULL);
				SQLGetData(sqlcn.SQLStatementHandle, 4, SQL_C_DEFAULT, &rs2, sizeof(rs2), NULL);

				std::cout << std::setw(3) << r1
					<< std::setw(20) << rs1
					<< std::setw(20) << r2
					<< std::setw(25) << rs2
					<< std::endl;
			}
			sqlcn.freestmt();
		}
				break;
			case (2): {
				std::cout << "Masukan Nama File : ";
				std::cin.ignore();
				std::getline(std::cin, mn);
				if (!confirm("Fungsi ini akan menimpa data di file " + mn + ", Lanjutkan ? [Y?N]"))
					break;

				data1.push_back({ "Id", "Kasir","Total Transaksi","Waktu Transaksi"});
				i++;

				sqlcn.exec("SELECT o.id, max(u.nama), sum(total_harga), convert(varchar, max(o.tgl_pembelian), 100)  \
					FROM[order] o, [detail_order] d, [tbl_user] u \
					WHERE o.id = d.id_order and o.id_kasir = u.id \
					GROUP BY(o.id) \
					");

				while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
					data1.push_back(std::vector<std::string>());
					SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &r1, 0, NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 3, SQL_INTEGER, &r2, 0, NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 4, SQL_C_DEFAULT, &rs2, sizeof(rs2), NULL);
					data1[i].push_back(std::to_string(r1));
					data1[i].push_back(rs1);
					data1[i].push_back(std::to_string(r2));
					data1[i].push_back(rs2);

					i++;
				}

				if (hapusFile(mn))
					std::cout << "Berhasil menghapus file lama" << std::endl;
				if (tulisFile(mn, ',', data1))
					std::cout << "Berhasil mengeksport file" << std::endl;

				sqlcn.freestmt();
			}
				break;
			case (3): {
				int harga, jenis, modal;
				std::cout << "----- Tambah/Edit Menu Makanan" << std::endl;
				std::cin.ignore();
				std::cout << "Masukan Nama Menu       : ";
				std::getline(std::cin, mn);
				std::cout << "Masukan Jenis Menu[1/2] : ";
				std::cin >> jenis;
				std::cout << "Masukan Harga           : ";
				std::cin >> harga;
				std::cout << "Masukan Modal           : ";
				std::cin >> modal;
				sqlcn.exec("INSERT INTO dbo.barang(id_jenis_barang, nama, harga, modal, is_valid) \
							VALUES (" + std::to_string(jenis) + ",'" + mn + "'," + std::to_string(harga) + "," + std::to_string(modal) +  ",1);");

				//free statement
				sqlcn.freestmt();
			}
				break;
			case (4): {
				sqlcn.exec("SELECT a.id, a.nama, b.jenis_barang, a.harga, (a.harga - a.modal) \
					FROM barang a  join jenis_barang b on a.id_jenis_barang = b.id \
					WHERE is_valid = 1; \
					");
				
				std::cout << std::setw(3) << "Id"
					<< std::setw(20) << "Menu"
					<< std::setw(20) << "Jenis Menu"
					<< std::setw(15) << "Harga"
					<< std::setw(15) << "Keuntungan" << std::endl;

				while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
					SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &r1, 0, NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 3, SQL_C_DEFAULT, &rs2, sizeof(rs2), NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 4, SQL_INTEGER, &r2, 0, NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 5, SQL_INTEGER, &r3, 0, NULL);

					std::cout	<< std::setw(3) << r1 
								<< std::setw(20) << rs1
								<< std::setw(20) << rs2
								<< std::setw(10) << r2
								<< std::setw(10) << r3 << std::endl;
				}
				sqlcn.freestmt();
			}
				break;
			case (5):{ 
				sqlcn.exec("SELECT * FROM jenis_barang;");
				std::cout << "----- EXPORT MENU MAKANAN -----" << std::endl;
				std::cout << std::setw(3) << "ID" << std::setw(20) << "Jenis Makanan" << std::endl;
				while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
					SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &r1, 0, NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
					std::cout << std::setw(3) << r1 << std::setw(20) << rs1 << std::endl;
				}
				sqlcn.freestmt();
				
			}
				break;
			case (6): {
				std::cout << "Masukan Nama File : ";
				std::cin.ignore();
				std::getline(std::cin, mn);
				if (!confirm("Fungsi ini akan menimpa data di file " + mn + ", Lanjutkan ? [Y?N]"))
					break;
				sqlcn.exec("SELECT * FROM [dbo].[func_keuntungan] ();");

				data1.push_back({ "Id","Menu","Jenis Menu","Harga","Keuntungan" });
				i++;
				while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
					data1.push_back(std::vector<std::string>());
					SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_INTEGER, &r1, 0, NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 2, SQL_C_DEFAULT, &rs1, sizeof(rs1), NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 3, SQL_INTEGER, &r2, 0, NULL);
					SQLGetData(sqlcn.SQLStatementHandle, 4, SQL_INTEGER, &r3, 0, NULL);
					data1[i].push_back(std::to_string(r1));
					data1[i].push_back(rs1);
					data1[i].push_back(std::to_string(r2));
					data1[i].push_back(std::to_string(r3));
					i++;
				}
				
				if(hapusFile(mn))
					std::cout << "Berhasil menghapus file lama" << std::endl;
				if(tulisFile(mn, ',', data1))
					std::cout << "Berhasil mengeksport file" << std::endl;
				
				sqlcn.freestmt();
			}
				break;
			case (7): {
				std::cout << "Fungsi dalam pengembangan " << std::endl;
			}
				break;
			case (8): {
				std::cout << "Fungsi dalam pengembangan " << std::endl; 
			}
				break;
		default:
			std::cout << "Fungsi tidak ada " << std::endl;
			break;
		}

		lanjut1 = confirm("Apakah Anda ingin tetap di menu Manager ? [Y/N] : ");
	} while (lanjut1);

	std::cout << "----------"<< std::endl;

	return 0;
}

// Menu control panel user
bool Usr(sqlcon sqlcn, Login lgn) {
	bool lanjut1;
	int menu;
	std::string rs1,rs2,rs3;
	int r1 = 3;
	do {
		std::cout << std::endl << "Fungsi Yang Tersedia : " << std::endl;
		std::cout << "1. Tambah User " << std::endl;
		std::cout << "2. Hapus User " << std::endl;
		std::cout << "Pilih Fungsi : ";
		std::cin >> menu;

		switch (menu) {
			case(1):
				lgn.regis(sqlcn);
				break;
			case(2):
				std::cout << "Fungsi dalam pengembangan " << std::endl;
				break;

			default:	
				std::cout << "Fungsi tidak ada " << std::endl;
				break;

		}

		lanjut1 = confirm("Apakah Anda ingin tetap di menu pengaturan user? [Y/N] : ");
	} while (lanjut1);
	std::cout << "----------" << std::endl;
	return 0;
}

void About() {
	std::cout << "About" << std::endl
		<< "Version 1.0.0" << std::endl
		<< "Program pendukung untuk proses pembelian makanan di restaurant fastfood" << std::endl
		<< "Pembuatan program ini menggunakan Bahasa Pemrograman C++ dan database SQL Server" << std::endl
		<< "GITHUB : https://github.com/imam123ci/Algo_KasirFastFood " << std::endl

		<< std::endl << std::endl

		<< "Credit : " << std::endl
		<< "-. Thomas Andreas        825190104" << std::endl
		<< "-. Muhammad Choirul I    825190105" << std::endl
		<< "-. David Raharja         825190027" << std::endl
		<< "-. Danielo				 825190062 " << std::endl
		<< "-. Juan Gilland Djoni    825190042" << std::endl
		<< std::endl << std::endl

		<< "Thanks to sources :" << std::endl
		<< "1. https://docs.microsoft.com/en-us/cpp/windows/overview-of-windows-programming-in-cpp?view=vs-2019" << std::endl
		<< "2. https://docs.microsoft.com/en-us/sql/sql-server/?view=sql-server-ver15" << std::endl
		<< "3. https://www.w3schools.com/sql/" << std::endl
		<< "4. stackoverflow.com" << std::endl
		<< "-----" << std::endl;

	return ;
}

void Help() {
	std::cout << "Help" << std::endl;
	std::cout << "List Menu" << std::endl;
	std::cout << "------------------------------" << std::endl;
	std::cout << "Menu         Deskripsi" << std::endl;
	std::cout << "Kasir        Menjalankan Program Kasir untuk mencatat dan menghitung pesanan" << std::endl;
	std::cout << "Manager      Berisi peragam program yang dikhususkan untuk manajemen restoran" << std::endl;
	std::cout << "Userctl      Program untuk mengatur user pada aplikasi " << std::endl;
	std::cout << "Signout      Keluar atau berganti akun aplikasi " << std::endl;
	std::cout << "Exit         Keluar aplikasi " << std::endl ;
	std::cout << "------------------------------ " << std::endl;

	std::cout << "Frequently Asked Question " << std::endl;
	std::cout << "1. Q : Bagaimana cara memilih menu? " << std::endl
		<< "   A : Tulis nama menu setelah tanda 'command#' lalu pencet enter " << std::endl;
	std::cout << "2. Q : Terjadi masalah saat pada database saya? " << std::endl
		<< "   A : Periksa kembali server dan koneksi internet Anda. Jika tetap tidak bisa hubungi Administrator " << std::endl;
	std::cout << "2. Q : Saya mendapa tulisan '	You don't have access to this menu'? " << std::endl
		<< "   A : Akun yang anda punya tidak memiliki akses yang cukup untuk mengakses menu tersebut " << std::endl;
	std::cout << "3. Q : Bagaimana cara menginput pesanan? " << std::endl
		<< "   A : Pilih menu kasir lalu ikuti instruksi lanjutan " << std::endl;
	std::cout << "4. Q : bagaimana cara pembuatan user baru " << std::endl
		<< "   A : Jika Anda administrator, pilih menu userctl. Jika tidak, hubungi administrator untuk info lanjut" << std::endl;

	std::cout << std::endl;
	return;
}

// Manage all available command or menu
// -----------------------------------------------
// FOCUS HERE 1
// ALL main command goes here
// return 1 to terminate program
// -----------------------------------------------
bool SwitchCommand(std::string cmd, sqlcon sqlcn, Login lgn) {
	//std::cout << std::endl;

	//Declare All Variable
	std::string maincmd;
	
	/*
	Split String is to fancy for now. 
	//std::vector<std::string> prm;
	//Split to vector using boost lib
	// from string to vector
	// ex : command1 opt opt -> vector<string> command1 | opt | opt
	//boost::split(prm, cmd, boost::is_any_of(" "));
	*/
	
	// converting string to lowercase
	std::for_each(cmd.begin(), cmd.end(), [](char& c) {
		c = ::tolower(c);
	});
	maincmd = cmd;


	// check if your command is right
	/*
		Tolong perhatikan bahwa semua menu dapat dilihat disini
		Vote : list menu bisa manual seperti ini atau menggunakan class

	*/
	//exit command
	if (maincmd == "exit")
		return TRUE;

	// test command
	else if (maincmd == "test")
		return Test();

	else if (maincmd == "kasir")
		return Kasir(sqlcn);

	else if (maincmd == "manager") {
		if (!RBAC(lgn, { 1,2 })) {
			std::cout << "You don't have access to this menu" << std::endl;
			return 0;
		}
		return Manager(sqlcn, lgn);
	}

	else if (maincmd == "userctl") {
		if (!RBAC(lgn, { 1 })) {
			std::cout << "You don't have access to this menu" << std::endl;
			return 0;
		}

		return Usr(sqlcn, lgn);
	}



	// Planning to add all default command

	// About
	else if (maincmd == "about")
		About();
	// Help Command
	else if (maincmd == "help")
		Help();
	

	// if Command not found
	else
		std::cout << "'" << maincmd << "' is not recognized" << std::endl;

	// Default value is FALSE
	// True will terminate program
	return FALSE;
}



int main()
{

	// Encrypt theese value for implementation
	// Any server configuration goes here
	std::string serverip = "94.237.72.228";
	std::string User = "sa";
	std::string Pass	 = "Inipassword123";
	std::string DBname = "FastFoodDB";
	std::string myconnection = "Driver={SQL Server};Server="+serverip+",1433;Database="+DBname+";Uid="+User+";Pwd="+Pass+";Connection Timeout=20";
	
	// Enstablish connection
	// Using SQLcon library
	sqlcon sqlcn(myconnection);
	// welcome message
	std::cout << "Welcome to Kasir FastFood 1.0 " << std::endl;
	std::cout << "Type help to list all available menu " << std::endl;
	

	// CLI state variable
	// Don't let this happen in real live
	// Set Login==True for no login
	bool login = FALSE;
	bool exit = FALSE;
	Login lgn;
	std::string cmd;

	// Graphic looping
	// end when switchcommand return 1
	do {
		//Check If user has login
		if (!lgn.status)
			lgn.login(sqlcn);
					
		//check if login attempt valid
		//if not force to shutdown
		if (!lgn.status)
			break;

		std::cout << "Command# ";
		std::getline(std::cin, cmd);
		if (cmd == "signout")
			lgn.signout();
		else
			exit = SwitchCommand(cmd, sqlcn, lgn);
	} while (!exit);

	sqlcn.closecon();
}