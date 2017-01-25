#include "PluginManager.h"

void PluginManager::DisplayPluginList(Player player)
{
    UIList list = UIList();
    list.Id = "PluginList";
    list.Title = "List of loaded plugins";
    list.IconStyle = "Icons128x128_1";
    list.IconSubstyle = "Custom";
    list.Columns.push_back(std::pair<std::string, int>("Name", 10));
    list.Columns.push_back(std::pair<std::string, int>("Version", 5));
    list.Columns.push_back(std::pair<std::string, int>("Author", 10));
    list.Columns.push_back(std::pair<std::string, int>("Description", 40));

    for(std::map<std::string, PluginInfo>::iterator pluginIt = plugins.begin(); pluginIt != plugins.end(); ++pluginIt)
    {
        std::string pluginName = pluginIt->first;
        PluginInfo plugin = pluginIt->second;

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("Name", pluginName));
        row.insert(std::pair<std::string, std::string>("Version", plugin.Version));
        row.insert(std::pair<std::string, std::string>("Author", plugin.Author));
        row.insert(std::pair<std::string, std::string>("Description", plugin.Description));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);

    /*struct PluginInfo
    {
        std::string Version;     /**< \brief Plugin version (from instance). *
        std::string Author;      /**< \brief Plugin author (from instance). *
        std::string Description; /**< \brief Plugin description (from instance). *

        void* Instance;          /**< \brief Instance of the plugin. *
        void* Handle;            /**< \brief Handle (from dlopen) of the plugin. *

        std::map<std::string, std::function<boost::any(boost::any)>> Methods; /**< \brief Callable methods of the plugin. *
    };*/
}
