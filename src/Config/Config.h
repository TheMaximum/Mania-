#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "../Permissions/Permission.h"

//* ServerConfig
/**
 * \brief Server connection settings.
 *
 * Editing information in this struct will not result in a change in the configuration file.
 */
struct ServerConfig
{
    std::string address;      /**< \brief Server address (either hostname or IP Address). */
    int port;                 /**< \brief Server XML-RPC port. */
    std::string username;     /**< \brief Username with which to authenticate on the server. */
    std::string password;     /**< \brief Password with which to authenticate on the server. */
};

//* DatabaseConfig
/**
 * \brief Database connection settings.
 *
 * Editing information in this struct will not result in a change in the configuration file.
 */
struct DatabaseConfig
{
    std::string address;      /**< \brief Database address (either hostname or IP Address). */
    int port;                 /**< \brief Database serverport. */
    std::string username;     /**< \brief Username with which to authenticate on the database server. */
    std::string password;     /**< \brief Password with which to authenticate on the database server. */
    std::string database;     /**< \brief Name of the database that should be used. */
};

//* ProgramConfig
/**
 * \brief Program settings.
 *
 * Editing information in this struct will not result in a change in the configuration file.
 */
struct ProgramConfig
{
    bool checkVersion = true; /**< \brief Check for updates on start-up. */
};

//* PluginConfig
/**
 * \brief Plugin settings.
 *
 * Editing information in this struct will not result in a change in the configuration file.
 */
struct PluginConfig
{
    std::string name;                            /**< \brief Plugin name (directory name). */
    std::map<std::string, std::string> settings; /**< \brief Plugin settings. */
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

    ServerConfig* Server;               /**< \brief Instance of ServerConfig which stores the server connection settings. */
    DatabaseConfig* Database;           /**< \brief Instance of DatabaseConfig which stores the database server connection settings. */
    ProgramConfig* Program;             /**< \brief Instance of ProgramConfig which stores program settings. */
    std::map<std::string, PluginConfig>* Plugins;   /**< \brief List of plugin configurations. */
    std::map<std::string, Permission>* Permissions; /**< \brief Overview of permissions from configuration. */

private:
    /*!
     * \brief Reads information from YAML file and puts this in configuration struct(s).
     */
    void parseConfig();

    /*!
     * \brief Reads permissions from YAML file.
     *
     * \param permissions YAML node that contains the permissions.
     */
    void parsePermissions(YAML::Node permissions);

    /*!
     * \brief Reads plugin information from YAML file.
     *
     * \param plugins YAML node that contains the plugin information.
     */
    void parsePlugins(YAML::Node plugins);

    std::string configFile; /**< \brief Name of the configuration file, set by constructor. */
};

#endif // CONFIG_H_
