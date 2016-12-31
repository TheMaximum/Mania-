#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <iostream>

#include <yaml-cpp/yaml.h>

//* ServerConfig
/**
 * \brief Server connection settings.
 *
 * Editing information in this struct will not result in a change in the configuration file.
 */
struct ServerConfig
{
    std::string address;  /**< \brief Server address (either hostname or IP Address). */
    int port;             /**< \brief Server XML-RPC port. */
    std::string username; /**< \brief Username with which to authenticate on the server. */
    std::string password; /**< \brief Password with which to authenticate on the server. */
};

//* DatabaseConfig
/**
 * \brief Database connection settings.
 *
 * Editing information in this struct will not result in a change in the configuration file.
 */
struct DatabaseConfig
{
    std::string address;  /**< \brief Database address (either hostname or IP Address). */
    int port;             /**< \brief Database serverport. */
    std::string username; /**< \brief Username with which to authenticate on the database server. */
    std::string password; /**< \brief Password with which to authenticate on the database server. */
    std::string database; /**< \brief Name of the database that should be used. */
};

//* Config
/**
 * \brief Reads and stores configuration information.
 */
class Config
{
public:
    /*!
     * \brief Calls parseConfig to parse the configuration file.
     *
     * \param configFile        Name of the configuration file.
     */
    Config(std::string configFile);

    /*!
     * Deletes and nullifies the serverconfig.
     */
    ~Config();

    ServerConfig* Server; /**< \brief Instance of ServerConfig which stores the server connection settings. */
    DatabaseConfig* Database; /**< \brief Instance of DatabaseConfig which stores the database server connection settings. */

private:
    /*!
     * \brief Reads information from YAML file and puts this in configuration struct(s).
     */
    void parseConfig();

    std::string configFile; /**< \brief Name of the configuration file, set by constructor. */
};

#endif // CONFIG_H_
