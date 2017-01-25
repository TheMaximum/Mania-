#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include <string>
#include <map>
#include <functional>

#include "ChatCommand.h"
#include "../Objects/Player.h"

//* CommandManager
/**
 * \brief Manages the chat commands.
 */
class CommandManager
{
public:
    /*!
     * \brief Initializes the command-maps.
     */
    CommandManager();

    /*!
     * \brief Registers the chat commands (from the plugins).
     *
     * \param plugin  Name of the plugin the commands belong to.
     * \param methods Plugin methods with command name.
     */
    int RegisterCommands(std::string plugin, std::map<std::string, ChatCommand> methods);

    /*!
     * \brief Registers the admin chat commands (from the plugins).
     *
     * \param plugin  Name of the plugin the commands belong to.
     * \param methods Plugin methods with admin command name.
     */
    int RegisterAdminCommands(std::string plugin, std::map<std::string, ChatCommand> methods);

    /*!
     * \brief Handles chat commands (from PlayerChat callback).
     *
     * \param player  Author of the command request.
     * \param text    Chat message.
     */
    void HandleCommand(Player player, std::string text);

private:
    std::map<std::string, ChatCommand> commands;      /**< \brief Map with normal chat commands. */
    std::map<std::string, ChatCommand> adminCommands; /**< \brief Map with admin chat commands. */
};

#endif // COMMANDMANAGER_H_
