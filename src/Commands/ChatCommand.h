#ifndef CHATCOMMAND_H_
#define CHATCOMMAND_H_

#include "../Objects/Player.h"
#include "../Permissions/Permission.h"

//* ChatCommand
/**
 * \brief Stuct containing information about a chat command.
 */
struct ChatCommand
{
    std::string PluginName;     /**< \brief Name of the plugin the command belongs to. */
    std::string PluginVersion;  /**< \brief Version of the plugin the command belongs to. */
    std::string PluginAuthor;   /**< \brief Author of the plugin the command belongs to. */

    std::string Command;        /**< \brief Chat command. */
    std::string Description;    /**< \brief Chat command description. */
    Permission Access = User;   /**< \brief Command access level. */
    std::function<void(Player, std::vector<std::string>)> Method; /**< \brief Chat command method. */
};

#endif // CHATCOMMAND_H_
