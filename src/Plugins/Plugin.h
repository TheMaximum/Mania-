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

class Plugin
{
public:
    virtual void Init() = 0;

    void SetLogging(Logging* loggingPtr)
    {
        logging = loggingPtr;
    }

    void SetServer(GbxRemote* serverPtr)
    {
        server = serverPtr;
    }

    void SetPlayers(std::map<std::string, Player>* playersPtr)
    {
        players = playersPtr;
    }

    void SetMaps(std::map<std::string, Map>* mapsPtr)
    {
        maps = mapsPtr;
    }

    std::vector<std::function<void(Player)>> MethodsPlayerConnect;
    std::vector<std::function<void(Player)>> MethodsPlayerDisconnect;

protected:
    Logging* logging;
    GbxRemote* server;
    std::map<std::string, Player>* players;
    std::map<std::string, Map>* maps;
};

#endif // PLUGIN_H_
