#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <functional>

#include "../Utils/Logging.h"
#include "../GbxRemote/GbxRemote.h"
#include "../Objects/Player.h"
#include "../Objects/Map.h"

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

    std::vector<std::function<void(Player)>> MethodsPlayerConnect;    /**< \brief Vector with functions for the PlayerConnect event. */
    std::vector<std::function<void(Player)>> MethodsPlayerDisconnect; /**< \brief Vector with functions for the PlayerDisconnect event. */

protected:
    Logging* logging;                       /**< \brief Logging instance. */
    GbxRemote* server;                      /**< \brief GbxRemote instance. */
    std::map<std::string, Player>* players; /**< \brief Playerlist instance. */
    std::map<std::string, Map>* maps;       /**< \brief Maplist instance. */
};

#endif // PLUGIN_H_
