#include <mysql.h>
#include "Database.hpp"

void Database::ConnectDatabase()
{
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, "localhost", "root", "Voting098", "VOTING_SYSTEM", 3306, NULL, 0);	
}

