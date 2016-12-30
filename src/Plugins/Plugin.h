#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <functional>

#include "../Events/Structs.h"
#include "../GbxRemote/GbxRemote.h"
#include "../Objects/Player.h"
#include "../Objects/Map.h"
#include "../Utils/Logging.h"

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
     * \brief Sets the logging instance.
     *
     * \param loggingPtr Pointer to the logging instance.
     */
    void SetLogging(Logging* loggingPtr)
    {
        logging = loggingPtr;
    }

    /*!
     * \brief Sets the server instance.
     *
     * \param serverPtr  Pointer to the GbxRemote instance.
     */
    void SetServer(GbxRemote* serverPtr)
    {
        server = serverPtr;
    }

    /*!
     * \brief Sets the playerlist instance.
     *
     * \param playersPtr Pointer to the playerlist instance.
     */
    void SetPlayers(std::map<std::string, Player>* playersPtr)
    {
        players = playersPtr;
    }

    /*!
     * \brief Sets the maplist instance.
     *
     * \param mapsPtr    Pointer to the maplist instance.
     */
    void SetMaps(std::map<std::string, Map>* mapsPtr)
    {
        maps = mapsPtr;
    }

    std::vector<std::function<void(Player)>> PlayerConnect;                     /**< \brief Vector with functions for the PlayerConnect event. */
    std::vector<std::function<void(Player)>> PlayerDisconnect;                  /**< \brief Vector with functions for the PlayerDisconnect event. */
    std::vector<std::function<void(Player, std::string, bool)>> PlayerChat;     /**< \brief Vector with functions for the PlayerChat event. */
    std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>> PlayerManialinkPageAnswer; /**< \brief Vector with functions for the PlayerManialinkPageAnswer event. */
    std::vector<std::function<void(std::string, std::string)>> Echo;            /**< \brief Vector with functions for the Echo event. */
    std::vector<std::function<void()>> BeginMatch;                              /**< \brief Vector with functions for the BeginMatch event. */
    std::vector<std::function<void(std::vector<PlayerRanking>, int)>> EndMatch; /**< \brief Vector with functions for the EndMatch event. */
    std::vector<std::function<void(Map)>> BeginMap;                             /**< \brief Vector with functions for the BeginMap event. */
    std::vector<std::function<void(int, std::string)>> StatusChanged;           /**< \brief Vector with functions for the StatusChanged event. */
    std::vector<std::function<void(Player, int, int, int)>> PlayerCheckpoint;   /**< \brief Vector with functions for the PlayerCheckpoint event. */
    std::vector<std::function<void(Player, int)>> PlayerFinish;                 /**< \brief Vector with functions for the PlayerFinish event. */
    std::vector<std::function<void(Player)>> PlayerIncoherence;                 /**< \brief Vector with functions for the PlayerIncoherence event. */
    std::vector<std::function<void(int, int, std::string, int)>> BillUpdated;   /**< \brief Vector with functions for the BillUpdated event. */
    std::vector<std::function<void(int, int, bool)>> MapListModified;           /**< \brief Vector with functions for the MapListModified event. */
    std::vector<std::function<void(Player)>> PlayerInfoChanged;                 /**< \brief Vector with functions for the PlayerInfoChanged event. */
    std::vector<std::function<void(std::string, std::string, std::string, std::string)>> VoteUpdated; /**< \brief Vector with functions for the VoteUpdated event. */

    std::string Version; /**< \brief Plugin version. */
    std::string Author;  /**< \brief Plugin author. */

protected:
    Logging* logging;                       /**< \brief Logging instance. */
    GbxRemote* server;                      /**< \brief GbxRemote instance. */
    std::map<std::string, Player>* players; /**< \brief Playerlist instance. */
    std::map<std::string, Map>* maps;       /**< \brief Maplist instance. */
};

#endif // PLUGIN_H_
