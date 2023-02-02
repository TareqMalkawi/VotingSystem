#pragma once

#include <mysql.h>

class Database
{
public:
	void ConnectDatabase();
	MYSQL* conn;
};


