#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <dlfcn.h>

#include "../Events/EventManager.h"
#include "Plugin.h"

struct PluginInfo
{
    std::string Name;
    Plugin* Instance;
    void* Handle;
};

class PluginManager
{
public:
    PluginManager(Logging* loggingPtr, GbxRemote* serverPtr, std::map<std::string, Player>* playersPtr, std::map<std::string, Map>* mapsPtr);
    ~PluginManager();

    void SetEventManager(EventManager* eventManagerPtr);

    void LoadPlugins(std::string pluginsFolder = "./plugins");

private:
    std::map<std::string, std::string> discoverPlugins(std::string pluginsFolder);

    //std::map<std::string, Plugin*> plugins;
    std::map<std::string, PluginInfo> plugins;
    EventManager* events;

    Logging* logging;
    GbxRemote* server;
    std::map<std::string, Player>* players;
    std::map<std::string, Map>* maps;
};

#endif // PLUGINMANAGER_H_
