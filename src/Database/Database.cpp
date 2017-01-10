#include "Database.h"

Database::Database(std::string serverAddress, int serverPort)
{
    address = serverAddress;
    port = serverPort;
}

Database::~Database()
{
    if(Driver != NULL)
    {
        Driver->threadEnd();
        Driver = NULL;
    }
}

sql::Connection* Database::Connect(std::string username, std::string password, std::string database)
{
    std::stringstream host;
    host << "tcp://" << address << ":" << port;

    Driver = get_driver_instance();
    Connection = Driver->connect(host.str(), username, password);

    Connection->setSchema(database);

    return Connection;
}
