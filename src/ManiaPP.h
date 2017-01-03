#ifndef MANIAPP_H_
#define MANIAPP_H_

#include <chrono>

#include "CallBacks/CallBackManager.h"
#include "Config/Version.h"
#include "Config/Config.h"
#include "Database/Database.h"
#include "Events/EventManager.h"
#include "GbxRemote/GbxRemote.h"
#include "Maps/MapList.h"
#include "Methods/Methods.h"
#include "Objects/Map.h"
#include "Objects/Player.h"
#include "Plugins/PluginManager.h"
#include "UI/UIManager.h"
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
     * \brief Connects and authenticates with the database server.
     */
    bool ConnectToDatabase();

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

private:
    Config* config;                         /**< \brief Contains the configuration for the software. */
    Logging* logging;                       /**< \brief Contains functions to log information. */
    GbxRemote* server;                      /**< \brief Contains the connection with the server and its methods. */
    std::map<std::string, Player>* players; /**< \brief Contains the list of players currently on the server. */
    MapList* maps;                          /**< \brief Contains the list of maps on the server. */

    EventManager* events;                   /**< \brief Contains the event manager. */
    PluginManager* plugins;                 /**< \brief Contains the plugin manager. */
    CallBackManager* callbacks;             /**< \brief Contains the callback manager. */
    Methods* methods;                       /**< \brief Server method caller. */
    UIManager* ui;                          /**< \brief UI manager (ManiaLink). */

    Database* db = NULL;                    /**< \brief Contains database connection and driver (needed on destruction). */
    sql::Connection* database = NULL;       /**< \brief Contains database driver. */

    ServerVersion serverVersion;            /**< \brief Struct with server version information. */
    SystemInfo systemInfo;                  /**< \brief Struct with system information. */

    /*!
     * \brief Gets a list of current players from the server.
     *
     * \todo Insert player into database if needed.
     */
    void retrievePlayerList();

    /*!
     * \brief Gets a list of current maps from the server.
     *
     * \todo Insert map into database if needed.
     */
    void retrieveMapList();
};

#endif // MANIAPP_H_
