
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

/*
// Do not care about this part
class Login
{
public:
	bool status = FALSE;
	void login(sqlcon sqlcn);
	void signout();
	void regis(std::string uname, std::string pass, int role);
};

void Login::login(sqlcon sqlcn)
{
	std::string u;
	std::string p;
	int i = 0;
	char result[250];
	std::string rs;

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


		SQLHANDLE queryansw = sqlcn.exec("DECLARE @responseMessage NVARCHAR(250);exec dbo.LoginUser \
							@pUsername='" + u + "', \
							@pPassword='" + p + "',\
							@pResponseMessage=@responseMessage OUTPUT;\
							select @responseMessage;");


		//std::cout << "Running Query : " << std::endl;

		while (SQLFetch(queryansw) == SQL_SUCCESS) {
			SQLGetData(queryansw, 1, SQL_C_DEFAULT, &result, sizeof(result), NULL);
		}

		rs = result;
		if (rs == "SUCCESS") {
			std::cin.ignore();
			std::cout << "Login success" << std::endl;
			this->status = TRUE;
		}
		else {
			std::cout << "Can't login : " << result << std::endl;
		}
	} while (rs != "SUCCESS" and i < 2);
	std::cin.ignore();
	this->status = FALSE;
}
void Login::signout() {
	this->status = FALSE;
}
void Login::regis(std::string uname, std::string pass, int role) {

}
*/


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
	bool login = TRUE;
	bool exit = FALSE;
	std::string cmd;

	// Graphic looping
	// end when switchcommand return 1
	do {

		//Check If user has login
		if (!login)
			//login = Login(sqlcn);
		//check if login attempt valid
		//if not force to shutdown
		if (!login)
			break;
		std::cout << " Command# ";
		std::getline(std::cin, cmd);
		exit = SwitchCommand(cmd);
	} while (!exit);

	sqlcn.closecon();
}