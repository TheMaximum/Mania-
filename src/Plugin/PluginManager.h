#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <dlfcn.h>

//__attribute__ ((visibility ("default")))
class PluginManager
{
public:
    void LoadPlugins(std::string pluginsFolder = "./plugins");

private:
    std::vector<std::string> discoverPlugins(std::string pluginsFolder);
};

#endif // PLUGINMANAGER_H_
