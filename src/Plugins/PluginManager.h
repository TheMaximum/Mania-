#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <dlfcn.h>

#include "../Methods/Methods.h"
#include "../Events/EventManager.h"
#include "Plugin.h"

//* PluginInfo
/**
 * \brief Struct with information about a plugin.
 */
struct PluginInfo
{
    std::string Version; /**< \brief Plugin version (from instance). */
    std::string Author;  /**< \brief Plugin author (from instance). */

    Plugin* Instance;    /**< \brief Instance of the plugin. */
    void* Handle;        /**< \brief Handle (from dlopen) of the plugin. */
};

//* PluginManager
/**
 * \brief Manages the plugins
 */
class PluginManager
{
public:
    /*!
     * \brief Initializes the pointers the plugins need.
     *
     * \param methodsPtr      Current instance of Methods.
     * \param playersPtr      Current instance of the playerlist.
     * \param mapsPtr         Current instance of the maplist.
     * \param databasePtr     Current instance of the database connection.
     * \param uiPtr           Current instance of the interface manager.
     */
    PluginManager(Methods* methodsPtr, std::map<std::string, Player>* playersPtr, MapList* mapsPtr, sql::Connection* databasePtr, UIManager* uiPtr);

    /*!
     * \brief Destructor closes all open plugins.
     */
    ~PluginManager();

    /*!
     * \brief Sets the event manager to be used for callbacks.
     *
     * \param eventManagerPtr Instance of EventManager.
     */
    void SetEventManager(EventManager* eventManagerPtr);

    /*!
     * \brief Loads the plugins in the specified folder.
     *
     * \param pluginsFolder   Relative path to the plugins folder.
     */
    void LoadPlugins(std::string pluginsFolder = "./plugins");

    /*!
     * \brief Calls all Init() functions in the plugins (called after loading all plugins).
     */
    void InitializePlugins();

private:
    /*!
     * \brief Disovers plugin (.so) files in the specified folder.
     */
    std::map<std::string, std::string> discoverPlugins(std::string pluginsFolder);

    std::map<std::string, PluginInfo> plugins; /**< \brief Vector of loaded plugins. */
    EventManager* events;                      /**< \brief Current instance of the EventManager. */
    Controller* controller;                    /**< \brief Current instance of the controller. */
};

#endif // PLUGINMANAGER_H_
