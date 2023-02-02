#include <iostream>
#include <string>
#include <ctime>

#include "Person.hpp"

constexpr int lowerBound = 10000;
constexpr int upperBound = 100000;

// database variables.
static int qstate;

static MYSQL_ROW row;
static MYSQL_RES* res;
static Database database;

bool Person::Login(std::string userName, std::string password_Id)
{
	database = Database();
	database.ConnectDatabase();

	// only for debuging reasons 
	// if (userName == "a" && password_Id == "1") return true;

	if (database.conn)
	{
		std::string query = "SELECT userName , id FROM PERSON";
		const char* q = query.c_str();
		qstate = mysql_query(database.conn, q);
		if (!qstate)
		{
			res = mysql_store_result(database.conn);
			while (row = mysql_fetch_row(res))
			{
				if (userName._Equal(row[0]) && password_Id._Equal(row[1]))
				{
					std::cout << row[0] << " Logged in to the system ...  \n";
					return true;
				}
				/*else
				{
					std::cout << "Wrong password ...  \n";
				}*/
			}
		}
	}

	return false;
}

std::string Person::GetUserNameFromDatabase(std::string password_Id)
{
	/*database = Database();
	database.ConnectDatabase();*/

	if (database.conn)
	{
		std::string query = "SELECT userName FROM PERSON WHERE id =" + password_Id;
		const char* q = query.c_str();
		qstate = mysql_query(database.conn, q);
		if (!qstate)
		{
			res = mysql_store_result(database.conn);
			if (row = mysql_fetch_row(res))
			{
				// std::cout << row[0] << "\n";
				return std::string(row[0]);
			}
		}
	}

	return std::string("not found");
}

std::string Person::GetUserTypeFromDatabase(std::string password_Id)
{
	{
		std::string query = "SELECT userType FROM PERSON WHERE id =" + password_Id;
		const char* q = query.c_str();
		qstate = mysql_query(database.conn, q);
		if (!qstate)
		{
			res = mysql_store_result(database.conn);
			if (row = mysql_fetch_row(res))
			{
				// std::cout << row[0] << "\n";
				return std::string(row[0]);
			}
		}
	}

	return std::string("undefined");
}

int Person::IdGenerator()
{
	// Genertaing unique user id ...
	srand(time(0));
	return (rand() % (upperBound - lowerBound + 1)) + lowerBound;
}