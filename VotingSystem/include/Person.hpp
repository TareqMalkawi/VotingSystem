#include <iostream>
#include <string>

#include "Database.hpp"

class Person
{
public:
	static bool Login(std::string userName, std::string password_Id);
	static std::string GetUserNameFromDatabase(std::string password_Id);
	static std::string GetUserTypeFromDatabase(std::string password_Id);
	static int IdGenerator();
};
