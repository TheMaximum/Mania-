#ifndef PLUGINHANDLER_H_
#define PLUGINHANDLER_H_

#include <string>
#include <map>
#include <functional>
#include <boost/any.hpp>

#include "../Utils/VersionCompare.h"

//* PluginInfo
/**
 * \brief Struct with information about a plugin.
 */
struct PluginInfo
{
    std::string Version; /**< \brief Plugin version (from instance). */
    std::string Author;  /**< \brief Plugin author (from instance). */

    void* Instance;      /**< \brief Instance of the plugin. */
    void* Handle;        /**< \brief Handle (from dlopen) of the plugin. */

    std::map<std::string, std::function<boost::any(boost::any)>> Methods; /**< \brief Callable methods of the plugin. */
};

class PluginHandler
{
public:
    /*!
     * \brief Handler constructor.
     *
     * \param pluginList Pointer to the controllers pluginlist.
     */
    PluginHandler(std::map<std::string, PluginInfo>* pluginList);

    /*!
     * \brief Checks if plugin is loaded (and is proper version).
     *
     * \param pluginName Name of the plugin.
     * \param minVersion Minimum version of the plugin (optional).
     */
    bool IsLoaded(std::string pluginName, std::string minVersion = "");

    /*!
     * \brief Calls method on a different plugin.
     *
     * \param pluginName Name of the plugin.
     * \param methodName Name of the method.
     * \param params     Parameters for the method.
     */
    boost::any CallMethod(std::string pluginName, std::string methodName, boost::any params);

private:
    std::map<std::string, PluginInfo>* plugins; /**< \brief Pointer to the plugin list. */
};

#endif // PLUGINHANDLER_H_
