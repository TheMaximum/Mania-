#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include <string>
#include <map>
#include <functional>

#include "ChatCommand.h"
#include "../Methods/Methods.h"
#include "../Objects/Player.h"
#include "../UI/UIManager.h"

//* CommandManager
/**
 * \brief Manages the chat commands.
 */
class CommandManager
{
public:
    /*!
     * \brief Initializes the command-maps.
     *
     * \param uiManager  Instance of the UI manager.
     * \param methodsPtr Instance of the Methods.
     */
    CommandManager(UIManager* uiManager, Methods* methodsPtr);

    /*!
     * \brief Registers the chat commands (from the plugins).
     *
     * \param plugin     Name of the plugin the commands belong to.
     * \param methods    Plugin methods with command name.
     */
    int RegisterCommands(std::string plugin, std::map<std::string, ChatCommand> methods);

    /*!
     * \brief Registers a chat command.
     *
     * \param command    Chat command.
     */
    void RegisterCommand(ChatCommand command);

    /*!
     * \brief Registers the admin chat commands (from the plugins).
     *
     * \param plugin     Name of the plugin the commands belong to.
     * \param methods    Plugin methods with admin command name.
     */
    int RegisterAdminCommands(std::string plugin, std::map<std::string, ChatCommand> methods);

    /*!
     * \brief Handles chat commands (from PlayerChat callback).
     *
     * \param player     Author of the command request.
     * \param text       Chat message.
     */
    void HandleCommand(Player player, std::string text);

    /*!
     * \brief Displays the command list to the user.
     *
     * \param player     List requester.
     * \param admin      Show admin commands or player commands?
     */
    void DisplayCommandList(Player player, bool admin = true);

private:
    std::map<std::string, ChatCommand> commands;      /**< \brief Map with normal chat commands. */
    std::map<std::string, ChatCommand> adminCommands; /**< \brief Map with admin chat commands. */

    UIManager* ui; /**< \brief Instance of the UI manager. */
    Methods* methods;
};

#endif // COMMANDMANAGER_H_
