#include "PluginManager.h"

typedef int (*main_t)();

void PluginManager::LoadPlugins(std::string pluginsFolder)
{
    std::vector<std::string> pluginFiles = discoverPlugins(pluginsFolder);
    for(int pluginId = 0; pluginId < pluginFiles.size(); pluginId++)
    {
        std::cout << "Plugin #" << pluginId << ": " << pluginFiles.at(pluginId) << std::endl;
        void* plugin = dlopen(pluginFiles.at(pluginId).c_str(), RTLD_NOW);
        if(plugin == NULL)
        {
            std::cout << "Could not load '" << pluginFiles.at(pluginId) << "'..." << std::endl;
        }
        else
        {
            main_t mainFunction = NULL;
            mainFunction = (main_t)dlsym(plugin, "main");
            if(mainFunction == NULL)
            {
                std::cout << "Could not call 'main'-function for '" << pluginFiles.at(pluginId) << "'..." << std::endl;
            }
            else
            {
                mainFunction();
            }
        }

        dlclose(plugin);
    }
}

std::vector<std::string> PluginManager::discoverPlugins(std::string pluginsFolder)
{
    std::vector<std::string> pluginFiles = std::vector<std::string>();

    DIR* pluginsDirectory = opendir(pluginsFolder.c_str());
    struct dirent* entry = readdir(pluginsDirectory);

    while(entry != NULL)
    {
        if(entry->d_type == DT_DIR)
        {
            if(std::string(entry->d_name).find(".") == std::string::npos)
            {
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
                            pluginFiles.push_back(filePath);
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
