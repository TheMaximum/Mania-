#include "CommandManager.h"

CommandManager::CommandManager()
{
    commands = std::map<std::string, ChatCommand>();
    adminCommands = std::map<std::string, ChatCommand>();
}

int CommandManager::RegisterCommands(std::string plugin, std::map<std::string, ChatCommand> methods)
{
    int functionsAdded = 0;
    for(std::map<std::string, ChatCommand>::iterator method = methods.begin(); method != methods.end(); ++method)
    {
        if(commands.find(method->first) == commands.end())
        {
            method->second.PluginName = plugin;
            commands.insert(std::pair<std::string, ChatCommand>(method->first, method->second));
            functionsAdded++;
        }
        else
        {
            std::cout << "[   WARN   ] Tried to register command '" << method->first << "', which is already registered!" << std::endl;
        }
    }
    return functionsAdded;
}

int CommandManager::RegisterAdminCommands(std::string plugin, std::map<std::string, ChatCommand> methods)
{
    int functionsAdded = 0;
    for(std::map<std::string, ChatCommand>::iterator method = methods.begin(); method != methods.end(); ++method)
    {
        if(adminCommands.find(method->first) == commands.end())
        {
            method->second.PluginName = plugin;
            adminCommands.insert(std::pair<std::string, ChatCommand>(method->first, method->second));
            functionsAdded++;
        }
        else
        {
            std::cout << "[   WARN   ] Tried to register admin-command '" << method->first << "', which is already registered!" << std::endl;
        }
    }
    return functionsAdded;
}

void CommandManager::HandleCommand(Player player, std::string text)
{
    std::stringstream ss(text);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> parameters(begin, end);
    std::string command = parameters.at(0).substr(1);
    parameters.erase(parameters.begin());

    if(command.find("admin") != std::string::npos)
    {
        std::string adminCommand = parameters.at(0);
        parameters.erase(parameters.begin());
        std::cout << "'" << player.Login << "' called admin-command '" << adminCommand << "' with # parameters: " << parameters.size() << std::endl;

        std::map<std::string, ChatCommand>::iterator commandIt = adminCommands.find(adminCommand);
        if(commandIt != adminCommands.end())
        {
            ChatCommand command = commandIt->second;
            std::function<void(Player, std::vector<std::string>)> commandMethod = command.Method;
            commandMethod(player, parameters);
        }
    }
    else
    {
        std::cout << "'" << player.Login << "' called command '" << command << "' with # parameters: " << parameters.size() << std::endl;

        std::map<std::string, ChatCommand>::iterator commandIt = commands.find(command);
        if(commandIt != commands.end())
        {
            ChatCommand command = commandIt->second;
            std::function<void(Player, std::vector<std::string>)> commandMethod = command.Method;
            commandMethod(player, parameters);
        }
    }
}
