#pragma once
#include <iostream>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

class sqlcon
{
public:
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

	sqlcon(std::string conn);
	void showSQLError(unsigned int handleType, const SQLHANDLE& handle);
	void closecon();
	void freestmt();
	SQLHANDLE exec(std::string SQLQuery);



private:
	std::string conn;
};

sqlcon::sqlcon(std::string conn)
{
	do {
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->SQLEnvHandle))
			// Allocates the environment
			break;

		if (SQL_SUCCESS != SQLSetEnvAttr(this->SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			// Sets attributes that govern aspects of environments
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, this->SQLEnvHandle, &this->SQLConnectionHandle))
			// Allocates the connection
			break;

		if (SQL_SUCCESS != SQLSetConnectAttr(this->SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
			// Sets attributes that govern aspects of connections
			break;

		SQLCHAR retConString[1024]; // Conection string
		switch (SQLDriverConnect(this->SQLConnectionHandle, NULL, (SQLCHAR*)conn.c_str(), SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
			// Establishes connections to a driver and a data source
		case SQL_SUCCESS:
			break;
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_NO_DATA_FOUND:
			this->showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			this->retCode = -1;
			break;
		case SQL_INVALID_HANDLE:
			this->showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			this->retCode = -1;
			break;
		case SQL_ERROR:
			this->showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			this->retCode = -1;
			break;
		default:
			break;
		}

		if (this->retCode == -1)
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, this->SQLConnectionHandle, &this->SQLStatementHandle))
			// Allocates the statement
			break;

		//std::cout << "Enstablish connection" << std::endl;
	} while (FALSE);
}



void sqlcon::showSQLError(unsigned int handleType, const SQLHANDLE& handle) {
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
		std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << std::endl;
}

void sqlcon::closecon() {
	SQLFreeHandle(SQL_HANDLE_STMT, this->SQLStatementHandle);
	SQLDisconnect(this->SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, this->SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, this->SQLEnvHandle);
}

SQLHANDLE sqlcon::exec(std::string SQLQuery) {

	//execute SQLQUERY
	if (SQL_SUCCESS != SQLExecDirect(this->SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
		// Executes a preparable statement
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		this->SQLStatementHandle = NULL;
		return(this->SQLStatementHandle);
	}
	else
	{
		return(this->SQLStatementHandle);
	}
}

void sqlcon::freestmt() {
	SQLFreeStmt(this->SQLStatementHandle, SQL_UNBIND);
	SQLFreeStmt(this->SQLStatementHandle, SQL_CLOSE);
	
}
