
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

//Include External Library Here



/*
bool Login(sqlcon sqlcn) {
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
			return 1;
		}
		else {
			std::cout << "Can't login : " << result << std::endl;
		}
	} while (rs != "SUCCESS" and i < 2);
	std::cin.ignore();
	return 0;
}
*/

// Manage all available command or menu
/*
bool SwitchCommand(std::string cmd) {
	//Declare All Variable
	std::string maincmd;
	std::vector<std::string> prm;

	//Split to vector using boost lib
	// from string to vector
	// ex : command1 opt opt -> vector<string> command1 | opt | opt
	//boost::split(prm, cmd, boost::is_any_of(" "));
	maincmd = prm.front();
	prm.erase(prm.begin());

	//here let's the switch begin
	if (maincmd == "exit")
		return TRUE;
	else if (maincmd == "About")
		std::cout << "print about Mark 0.1" << std::endl;
	else if (maincmd == "help")
		std::cout << "print help" << std::endl;
	else
		std::cout << "'" << maincmd << "' is not recognized" << std::endl;

	return FALSE;
}
*/


int main()
{
	std::string myconnection = "Driver={SQL Server};Server=94.237.76.197,1433;Database=ContohDB;Uid=sa;Pwd=Inipassword123;Connection Timeout=20";
	sqlcon sqlcn(myconnection);
	std::cout << "Welcome to [system-cli] 0.1 " << std::endl;
	bool login = FALSE;
	bool exit = FALSE;
	std::string cmd;
	do {
		//Check If user has login
		if (!login)
			//login = Login(sqlcn);
		//check if login attempt valid
		//if not force to shutdown
		if (!login)
			break;


		std::cout << "# Username : ";
		std::getline(std::cin, cmd);
		//exit = SwitchCommand(cmd);
	} while (!exit);




	sqlcn.closecon();
}