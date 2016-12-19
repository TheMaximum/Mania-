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

    ServerConfig* Server; /**< \brief Instance of ServerConfig which stores the server connection settings. */

private:
    /*!
     * \brief Reads information from YAML file and puts this in configuration struct(s).
     */
    void parseConfig();

    std::string configFile; /**< \brief Name of the configuration file, set by constructor. */
};

#endif // CONFIG_H_
