#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <stdio.h>
#include <cstring>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <functional>
#include <boost/any.hpp>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "../Events/Structs.h"
#include "../Maps/MapList.h"
#include "../Methods/Methods.h"
#include "../Objects/Player.h"
#include "../Objects/Map.h"
#include "../UI/UIManager.h"
#include "../Utils/Logging.h"

#include "PluginHandler.h"

//* Controller
/**
 * \brief Struct with all instances needed for plugins.
 */
struct Controller
{
    Methods* Server;                        /**< \brief Methods instance. */
    std::map<std::string, Player>* Players; /**< \brief Playerlist instance. */
    MapList* Maps;                          /**< \brief Maplist instance. */
    sql::Connection* Database;              /**< \brief Database instance. */
    UIManager* UI;                          /**< \brief UI manager instance. */
    PluginHandler* Plugins;                 /**< \brief Plugin handler instance. */
};

//* Plugin
/**
 * \brief Plugin interface, inherited by all plugins.
 */
class Plugin
{
public:
    /*!
     * \brief Initializes plugin (called after loading all plugins).
     */
    virtual void Init() = 0;

    /*!
     * \brief Sets the controller instance.
     *
     * \param controllerPtr Pointer to the controller instance.
     */
    void SetController(Controller* controllerPtr)
    {
        controller = controllerPtr;
    }

    std::map<std::string, std::string> Settings;                                /**< \brief Plugin settings (from the config file). */

    std::vector<std::function<void(Player)>> PlayerConnect;                     /**< \brief Vector with functions for the PlayerConnect event. */
    std::vector<std::function<void(Player)>> PlayerDisconnect;                  /**< \brief Vector with functions for the PlayerDisconnect event. */
    std::vector<std::function<void(Player, std::string)>> PlayerChat;           /**< \brief Vector with functions for the PlayerChat event. */
    std::vector<std::function<void(std::string, std::string)>> Echo;            /**< \brief Vector with functions for the Echo event. */
    std::vector<std::function<void()>> BeginMatch;                              /**< \brief Vector with functions for the BeginMatch event. */
    std::vector<std::function<void(std::vector<PlayerRanking>, int)>> EndMatch; /**< \brief Vector with functions for the EndMatch event. */
    std::vector<std::function<void(Map)>> BeginMap;                             /**< \brief Vector with functions for the BeginMap event. */
    std::vector<std::function<void(Map)>> EndMap;                               /**< \brief Vector with functions for the EndMap event. */
    std::vector<std::function<void(int, std::string)>> StatusChanged;           /**< \brief Vector with functions for the StatusChanged event. */
    std::vector<std::function<void(Player, int, int, int)>> PlayerCheckpoint;   /**< \brief Vector with functions for the PlayerCheckpoint event. */
    std::vector<std::function<void(Player, int)>> PlayerFinish;                 /**< \brief Vector with functions for the PlayerFinish event. */
    std::vector<std::function<void(Player)>> PlayerIncoherence;                 /**< \brief Vector with functions for the PlayerIncoherence event. */
    std::vector<std::function<void(int, int, std::string, int)>> BillUpdated;   /**< \brief Vector with functions for the BillUpdated event. */
    std::vector<std::function<void(int, int, bool)>> MapListModified;           /**< \brief Vector with functions for the MapListModified event. */
    std::vector<std::function<void(Player)>> PlayerInfoChanged;                 /**< \brief Vector with functions for the PlayerInfoChanged event. */
    std::vector<std::function<void(std::string, std::string, std::string, std::string)>> VoteUpdated; /**< \brief Vector with functions for the VoteUpdated event. */

    std::map<std::string, std::function<void(Player, std::vector<std::string>)>> Commands =
        std::map<std::string, std::function<void(Player, std::vector<std::string>)>>(); /**< \brief Map with normal chat commands for the plugin. */
    std::map<std::string, std::function<void(Player, std::vector<std::string>)>> AdminCommands =
        std::map<std::string, std::function<void(Player, std::vector<std::string>)>>(); /**< \brief Map with admin chat commands for the plugin. */
    std::map<std::string, std::function<boost::any(boost::any)>> Methods =
        std::map<std::string, std::function<boost::any(boost::any)>>();                   /**< \brief Map with callable methods for the plugin. */

    std::string Version; /**< \brief Plugin version. */
    std::string Author;  /**< \brief Plugin author. */

protected:
    Controller* controller;                  /**< \brief Struct with needed instances. */

    /*!
     * \brief Register chat command.
     *
     * \param name   Chat command name.
     * \param method Method to be called for the command.
     */
    void RegisterCommand(std::string name, std::function<void(Player, std::vector<std::string>)> method)
    {
        Commands.insert(std::pair<std::string, std::function<void(Player, std::vector<std::string>)>>(name, method));
    }

    /*!
     * \brief Register admin chat command.
     *
     * \param name   Admin chat command name.
     * \param method Method to be called for the command.
     */
    void RegisterAdminCommand(std::string name, std::function<void(Player, std::vector<std::string>)> method)
    {
        AdminCommands.insert(std::pair<std::string, std::function<void(Player, std::vector<std::string>)>>(name, method));
    }

    /*!
     * \brief Register callable method.
     *
     * \param name   Method name.
     * \param method Callable method.
     */
    void RegisterCallableMethod(std::string name, std::function<boost::any(boost::any)> method)
    {
        Methods.insert(std::pair<std::string, std::function<boost::any(boost::any)>>(name, method));
    }
};

#endif // PLUGIN_H_
