#include "CommandManager.h"

void CommandManager::DisplayCommandList(Player player, bool admin)
{
    std::map<std::string, ChatCommand> commandList;

    UIList list = UIList();
    if(admin)
    {
        commandList = adminCommands;
        list.Id = "AdminCommandList";
        list.Title = "List of admin chat commands";
    }
    else
    {
        commandList = commands;
        list.Id = "CommandList";
        list.Title = "List of chat commands";
    }

    list.IconStyle = "Icons128x128_1";
    list.IconSubstyle = "Custom";
    list.Columns.push_back(std::pair<std::string, int>("Command", 10));
    list.Columns.push_back(std::pair<std::string, int>("Description", 40));
    list.Columns.push_back(std::pair<std::string, int>("Plugin", 10));
    if(admin)
    {
        list.Columns.push_back(std::pair<std::string, int>("Access", 10));
    }

    for(std::map<std::string, ChatCommand>::iterator commandIt = commandList.begin(); commandIt != commandList.end(); ++commandIt)
    {
        ChatCommand command = commandIt->second;

        if(player.AccessLevel >= command.Access)
        {
            std::map<std::string, std::string> row = std::map<std::string, std::string>();
            row.insert(std::pair<std::string, std::string>("Command", command.Command));
            row.insert(std::pair<std::string, std::string>("Description", command.Description));
            row.insert(std::pair<std::string, std::string>("Plugin", command.PluginName));
            if(admin)
            {
                row.insert(std::pair<std::string, std::string>("Access", Permissions::Text(command.Access)));
            }
            list.Rows.push_back(row);
        }
    }

    ui->DisplayList(list, player);
}
