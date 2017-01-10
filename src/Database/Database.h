#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <sstream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

//* Database
/**
 * \brief Handles the connection to the database.
 */
class Database
{
public:
    /*!
     * \brief Sets a few settings.
     *
     * \param serverAddress IP Address of the database server.
     * \param serverPort    Port of the database server.
     */
    Database(std::string serverAddress, int serverPort = 3306);

    /*!
     * Deconstructor closes driver thread.
     */
    ~Database();

    /*!
     * \brief Connects to the database.
     *
     * \param username      Database user name.
     * \param password      Database user password.
     * \param database      Database name.
     */
    sql::Connection* Connect(std::string username, std::string password, std::string database);

    sql::Driver* Driver;         /**< \brief Database server driver. */
    sql::Connection* Connection; /**< \brief Database server driver. */

private:
    std::string address;         /**< \brief IP Address of the database server. */
    int port;                    /**< \brief Port of the database server. */
};

#endif // DATABASE_H_
