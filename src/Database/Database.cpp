#include "Database.h"

Database::Database(std::string serveraddress, int serverport)
{
    address = serveraddress;
    port = serverport;
}

sql::Connection* Database::Connect(std::string username, std::string password, std::string database)
{
    sql::Driver* driver;
    sql::Connection* connection;

    std::stringstream host;
    host << "tcp://" << address << ":" << port;

    driver = get_driver_instance();
    connection = driver->connect(host.str(), username, password);

    connection->setSchema(database);

    return connection;
}
