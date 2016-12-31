#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <sstream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class Database
{
public:
    Database(std::string serveraddress, int serverport = 3306);
    sql::Connection* Connect(std::string username, std::string password, std::string database);

private:
    std::string address;
    int port;
};

#endif // DATABASE_H_
