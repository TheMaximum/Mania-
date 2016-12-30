#ifndef MANIAPP_H_
#define MANIAPP_H_

#include <chrono>

#include "Config/Version.h"
#include "Config/Config.h"
#include "Events/EventManager.h"
#include "GbxRemote/GbxRemote.h"
#include "Methods/Methods.h"
#include "Objects/Map.h"
#include "Objects/Player.h"
#include "Plugins/PluginManager.h"
#include "Utils/Logging.h"

//* ManiaPP
/**
 * \brief Main class.
 */
class ManiaPP
{
public:
    /*!
     * \brief Sets up the several variables needed to run the software.
     */
    ManiaPP();

    /*!
     * \brief Destructs the pointers in the main class.
     */
    ~ManiaPP();

    /*!
     * \brief Connects and authenticates with the server.
     */
    bool ConnectToServer();

    /*!
     * \brief Prints information about the server.
     */
    void PrintServerInfo();

    /*!
     * \brief Performing our tasks ...
     */
    void MainLoop();

    /*!
     * \brief Disconnects the software from the server.
     */
    void Terminate();

    /*!
     * \brief Prints a GbxResponseParameter (for DEBUG purposes).
     *
     * \param parameter     Parameter to be printed.
     * \param paramId       Number of the parameter.
     * \param spaces        Spaces to be put before the parameter information.
     * \param parameterName Name of the parameter (struct value only).
     */
    void PrintParameter(GbxResponseParameter parameter, int paramId, std::string spaces = "    ", std::string parameterName = "");

private:
    Config* config;                         /**< \brief Contains the configuration for the software. */
    Logging* logging;                       /**< \brief Contains functions to log information. */
    GbxRemote* server;                      /**< \brief Contains the connection with the server and its methods. */
    std::map<std::string, Player>* players; /**< \brief Contains the list of players currently on the server. */
    std::map<std::string, Map>* maps;       /**< \brief Contains the list of players currently on the server. */

    EventManager* events;                   /**< \brief Contains the event manager. */
    PluginManager* plugins;                 /**< \brief Contains the plugin manager. */
    Methods* methods;                       /**< \brief Server method caller. */

    ServerVersion serverVersion;            /**< \brief Struct with server version information. */
    SystemInfo systemInfo;                  /**< \brief Struct with system information. */

    /*!
     * \brief Gets a list of current players from the server.
     */
    void retrievePlayerList();

    /*!
     * \brief Gets a list of current maps from the server.
     */
    void retrieveMapList();
};

#endif // MANIAPP_H_
