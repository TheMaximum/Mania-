#include "PluginHandler.h"

PluginHandler::PluginHandler(std::map<std::string, PluginInfo>* pluginList)
{
    plugins = pluginList;
}

bool PluginHandler::IsLoaded(std::string pluginName, std::string minVersion)
{
    bool pluginFound = false;

    std::map<std::string, PluginInfo>::iterator pluginIt = plugins->find(pluginName);
    if(pluginIt == plugins->end())
        return pluginFound;

    if(minVersion != "")
    {
        if(VersionCompare::NotOlderThan(minVersion, pluginIt->second.Version))
        {
            pluginFound = true;
        }
    }
    else
    {
        pluginFound = true;
    }

    return pluginFound;
}

boost::any PluginHandler::CallMethod(std::string pluginName, std::string methodName, boost::any params)
{
    boost::any result = NULL;

    std::map<std::string, PluginInfo>::iterator pluginIt = plugins->find(pluginName);
    if(pluginIt == plugins->end())
        return result;

    std::map<std::string, std::function<boost::any(boost::any)>>::iterator methodIt = pluginIt->second.Methods.find(methodName);
    if(methodIt == pluginIt->second.Methods.end())
        return result;

    std::function<boost::any(boost::any)> method = methodIt->second;
    result = method(params);

    return result;
}
