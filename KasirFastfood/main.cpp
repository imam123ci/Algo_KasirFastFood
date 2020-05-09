
//Required SQL Lib Here
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include "SQLcon.h"

//Include STL LIB Here
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


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


		sqlcn.exec("DECLARE @responseMessage NVARCHAR(250);exec Lgn.LoginUser \
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
					select u.id, u.username, r.id from \
					Lgn.TblUser u Join Lgn.TblRoleUser r\
					ON u.id_roleuser = r.id and u.id ="+rs+"; ");
			//Fetch Data
			int userid;
			char username[50];
			int userrole;
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
		int  r = 3;

		//input variable
		std::cout << "> New username  : " ;
		std::cin >> uu;
		std::cout << "> User password : " ;
		std::cin >> up;
		std::cout << "> User role (3) : " ;
		std::cin >> r;
		//store to DB
		sqlcn.exec("\
			DECLARE @responseMessage NVARCHAR(250);\
			EXEC Lgn.SignUpUser\
				@pUsername = '"+uu+"',\
				@pPassword = '"+up+"',\
				@pRole = "+ std::to_string(r)+",\
				@pResponseMessage = @responseMessage OUTPUT;\
			SELECT @responseMessage;\
		");

		sqlcn.freestmt();
	}


	return;

}


// -----------------------------------------------
// FOCUS HERE 2
// ALL main command goes here
// All function has bool type
// Return False only, True value for terminate Program
// -----------------------------------------------

bool Test() {
	std::cout << "Fungsi perintah jalan di fungsi ini" << std::endl;
	return FALSE;
}

void get_cnth(sqlcon sqlcn) {
	char r1[250];
	char r2[250];

	sqlcn.exec("select * from dbo.cnth");
	while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
		SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_C_DEFAULT, &r1, sizeof(r1), NULL);
	}

	sqlcn.freestmt();

	sqlcn.exec("select * from dbo.cnth");
	while (SQLFetch(sqlcn.SQLStatementHandle) == SQL_SUCCESS) {
		SQLGetData(sqlcn.SQLStatementHandle, 1, SQL_C_DEFAULT, &r2, sizeof(r2), NULL);
	}

	sqlcn.freestmt();

}


// Manage all available command or menu
// -----------------------------------------------
// FOCUS HERE 1
// ALL main command goes here
// -----------------------------------------------
bool SwitchCommand(std::string cmd) {
	std::cout << std::endl;

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

	/*
	// Planning to add all default command
	// Such as login etc
	// Sign Out
	else if (maincmd == "signout")
		return "Signout";
	// About
	else if (maincmd == "about")
		std::cout << "print about Mark 0.1" << std::endl;
	// Help Command
	else if (maincmd == "help")
		std::cout << "print help" << std::endl;
	*/

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
	std::string DBname = "ContohDB";
	std::string myconnection = "Driver={SQL Server};Server="+serverip+",1433;Database="+DBname+";Uid="+User+";Pwd="+Pass+";Connection Timeout=20";
	
	// Enstablish connection
	// Using SQLcon library
	sqlcon sqlcn(myconnection);
	// welcome message
	std::cout << "Welcome to Kasir FastFood 0.1 " << std::endl;
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

		std::cout << " Command# ";
		std::getline(std::cin, cmd);
		exit = SwitchCommand(cmd);
	} while (!exit);

	sqlcn.closecon();
}