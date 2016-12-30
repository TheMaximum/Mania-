#include "PluginManager.h"

PluginManager::PluginManager(Logging* loggingPtr, Methods* methodsPtr, std::map<std::string, Player>* playersPtr, std::map<std::string, Map>* mapsPtr)
{
    plugins = std::map<std::string, PluginInfo>();
    events = NULL;

    logging = loggingPtr;
    methods = methodsPtr;
    players = playersPtr;
    maps = mapsPtr;
}

PluginManager::~PluginManager()
{
    std::cout << "[ ======= ] Closing plugins ... " << std::endl;
    for(std::map<std::string, PluginInfo>::iterator pluginId = plugins.begin(); pluginId != plugins.end(); ++pluginId)
    {
        PluginInfo pluginInfo = pluginId->second;
        std::cout << "[         ] Closing plugin '" << pluginId->first << "' ... " << '\r' << std::flush;
        dlclose(pluginInfo.Handle);
        Logging::PrintOKFlush();
    }
    std::cout << "[ ======= ] All plugins closed." << std::endl;
}

void PluginManager::SetEventManager(EventManager* eventManagerPtr)
{
    events = eventManagerPtr;
}

typedef Plugin* (*startplugin_t)();

void PluginManager::LoadPlugins(std::string pluginsFolder)
{
    std::cout << "[         ] Discovering plugins ... " << '\r' << std::flush;
    std::map<std::string, std::string> pluginFiles = discoverPlugins(pluginsFolder);
    std::cout << "[   \033[0;32mOK.\033[0;0m   ] Discovered plugins: " << pluginFiles.size() << " found." << std::endl;
    std::cout << "[ ======= ] Loading plugins ... " << std::endl;

    for(std::map<std::string, std::string>::iterator pluginId = pluginFiles.begin(); pluginId != pluginFiles.end(); ++pluginId)
    {
        std::cout << "[         ] Loading plugin '" << pluginId->first << "' ... " << '\r' << std::flush;
        void* pluginHandle = dlopen(pluginId->second.c_str(), RTLD_NOW);
        if(pluginHandle == NULL)
        {
            Logging::PrintFailedFlush();

            GbxError* error = new GbxError();
            error->number = -201;
            std::stringstream message;
            message << "Could not load '" << pluginId->first << "'...";
            error->message = message.str();
            logging->PrintError(error);
        }
        else
        {
            startplugin_t startPlugin = NULL;
            startPlugin = (startplugin_t)dlsym(pluginHandle, "startPlugin");
            if(startPlugin == NULL)
            {
                Logging::PrintFailedFlush();

                GbxError* error = new GbxError();
                error->number = -202;
                std::stringstream message;
                message << "Could not call 'startPlugin'-function for '" << pluginId->first << "'...";
                error->message = message.str();
                logging->PrintError(error);
            }
            else
            {
                Plugin* plugin = startPlugin();
                plugin->SetLogging(logging);
                plugin->SetMethods(methods);
                plugin->SetPlayers(players);
                plugin->SetMaps(maps);

                std::cout << "[   \033[0;32mOK.\033[0;0m   ] Loaded plugin '" << pluginId->first << "': v" << plugin->Version << " by " << plugin->Author << "." << std::endl;

                if(events != NULL)
                {
                    std::cout << "[         ] Loading events for '" << pluginId->first << "' ... " << '\r' << std::flush;
                    int eventCount = 0;
                    eventCount += events->RegisterPlayerConnect(plugin->PlayerConnect);
                    eventCount += events->RegisterPlayerDisconnect(plugin->PlayerDisconnect);
                    eventCount += events->RegisterPlayerChat(plugin->PlayerChat);
                    eventCount += events->RegisterPlayerManialinkPageAnswer(plugin->PlayerManialinkPageAnswer);
                    eventCount += events->RegisterEcho(plugin->Echo);
                    eventCount += events->RegisterBeginMatch(plugin->BeginMatch);
                    eventCount += events->RegisterEndMatch(plugin->EndMatch);
                    eventCount += events->RegisterBeginMap(plugin->BeginMap);
                    eventCount += events->RegisterEndMap(plugin->EndMap);
                    eventCount += events->RegisterStatusChanged(plugin->StatusChanged);
                    eventCount += events->RegisterPlayerCheckpoint(plugin->PlayerCheckpoint);
                    eventCount += events->RegisterPlayerFinish(plugin->PlayerFinish);
                    eventCount += events->RegisterPlayerIncoherence(plugin->PlayerIncoherence);
                    eventCount += events->RegisterBillUpdated(plugin->BillUpdated);
                    eventCount += events->RegisterMapListModified(plugin->MapListModified);
                    eventCount += events->RegisterPlayerInfoChanged(plugin->PlayerInfoChanged);
                    eventCount += events->RegisterVoteUpdated(plugin->VoteUpdated);

                    std::cout << "[   \033[0;32mOK.\033[0;0m   ] Loaded events for '" << pluginId->first << "': " << eventCount << " found." << std::endl;
                }

                plugins.insert(std::pair<std::string, PluginInfo>(pluginId->first, { plugin->Version, plugin->Author, plugin, pluginHandle }));
            }
        }
    }

    std::cout << "[ ======= ] Plugins: " << plugins.size() << " loaded." << std::endl;
}

void PluginManager::InitializePlugins()
{
    std::cout << "[ ======= ] Initializing plugins ... " << std::endl;
    for(std::map<std::string, PluginInfo>::iterator pluginId = plugins.begin(); pluginId != plugins.end(); ++pluginId)
    {
        PluginInfo pluginInfo = pluginId->second;
        Plugin* plugin = pluginInfo.Instance;
        plugin->Init();
    }
    std::cout << "[ ======= ] Plugins initialized." << std::endl;
}

std::map<std::string, std::string> PluginManager::discoverPlugins(std::string pluginsFolder)
{
    std::map<std::string, std::string> pluginFiles = std::map<std::string, std::string>();

    DIR* pluginsDirectory = opendir(pluginsFolder.c_str());
    struct dirent* entry = readdir(pluginsDirectory);

    while(entry != NULL)
    {
        if(entry->d_type == DT_DIR)
        {
            if(std::string(entry->d_name).find(".") == std::string::npos)
            {
                std::string pluginName = entry->d_name;

                std::string pluginFolder = pluginsFolder;
                pluginFolder.append("/").append(entry->d_name);

                DIR* pluginDirectory = opendir(pluginFolder.c_str());
                struct dirent* pluginEntry = readdir(pluginDirectory);
                while(pluginEntry != NULL)
                {
                    if(pluginEntry->d_type != DT_DIR)
                    {
                        if(std::string(pluginEntry->d_name).find(".so") != std::string::npos)
                        {
                            std::string filePath = pluginFolder;
                            filePath.append("/").append(pluginEntry->d_name);
                            pluginFiles.insert(std::pair<std::string, std::string>(pluginName, filePath));
                        }
                    }

                    pluginEntry = readdir(pluginDirectory);
                }

                closedir(pluginDirectory);
            }
        }

        entry = readdir(pluginsDirectory);
    }

    closedir(pluginsDirectory);

    return pluginFiles;
}
