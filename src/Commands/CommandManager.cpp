#include "CommandManager.h"

CommandManager::CommandManager()
{
    commands = std::map<std::string, std::function<void(Player, std::vector<std::string>)>>();
    adminCommands = std::map<std::string, std::function<void(Player, std::vector<std::string>)>>();
}

int CommandManager::RegisterCommands(std::map<std::string, std::function<void(Player, std::vector<std::string>)>> methods)
{
    int functionsAdded = 0;
    for(std::map<std::string, std::function<void(Player, std::vector<std::string>)>>::iterator method = methods.begin(); method != methods.end(); ++method)
    {
        if(commands.find(method->first) == commands.end())
        {
            commands.insert(std::pair<std::string, std::function<void(Player, std::vector<std::string>)>>(method->first, method->second));
            functionsAdded++;
        }
        else
        {
            std::cout << "[ WARNING ] Tried to register command '" << method->first << "', which is already registered!" << std::endl;
        }
    }
    return functionsAdded;
}

int CommandManager::RegisterAdminCommands(std::map<std::string, std::function<void(Player, std::vector<std::string>)>> methods)
{
    int functionsAdded = 0;
    for(std::map<std::string, std::function<void(Player, std::vector<std::string>)>>::iterator method = methods.begin(); method != methods.end(); ++method)
    {
        if(commands.find(method->first) == commands.end())
        {
            adminCommands.insert(std::pair<std::string, std::function<void(Player, std::vector<std::string>)>>(method->first, method->second));
            functionsAdded++;
        }
        else
        {
            std::cout << "[ WARNING ] Tried to register admin-command '" << method->first << "', which is already registered!" << std::endl;
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

        std::map<std::string, std::function<void(Player, std::vector<std::string>)>>::iterator commandIt = adminCommands.find(adminCommand);
        if(commandIt != adminCommands.end())
        {
            std::function<void(Player, std::vector<std::string>)> commandMethod = commandIt->second;
            commandMethod(player, parameters);
        }
    }
    else
    {
        std::cout << "'" << player.Login << "' called command '" << command << "' with # parameters: " << parameters.size() << std::endl;

        std::map<std::string, std::function<void(Player, std::vector<std::string>)>>::iterator commandIt = commands.find(command);
        if(commandIt != commands.end())
        {
            std::function<void(Player, std::vector<std::string>)> commandMethod = commandIt->second;
            commandMethod(player, parameters);
        }
    }
}
