#include "AdminPlayersPlugin.h"

AdminPlayersPlugin::AdminPlayersPlugin()
{
    Version = "0.1.0";
    Author = "TheM";
    Description = "Provides administrating options for players.";

    RegisterAdminCommand("kick", "Kick a player from the server.", Permission::Operator, [this](Player player, std::vector<std::string> parameters) { ChatKick(player, parameters); });
    RegisterAdminCommand("ban", "Ban a player from the server.", Permission::Admin, [this](Player player, std::vector<std::string> parameters) { ChatBan(player, parameters); });
    RegisterAdminCommand("unban", "Unban a player from the server.", Permission::Admin, [this](Player player, std::vector<std::string> parameters) { ChatUnban(player, parameters); });
    RegisterAdminCommand("black", "Blacklist a player from the server.", Permission::Admin, [this](Player player, std::vector<std::string> parameters) { ChatBlackList(player, parameters); });
    RegisterAdminCommand("unblack", "Un-blacklist a player from the server.", Permission::Admin, [this](Player player, std::vector<std::string> parameters) { ChatUnBlackList(player, parameters); });
    RegisterAdminCommand("blacklist", "Blacklist a player from the server.", Permission::Admin, [this](Player player, std::vector<std::string> parameters) { ChatBlackList(player, parameters); });
    RegisterAdminCommand("unblacklist", "Un-blacklist a player from the server.", Permission::Admin, [this](Player player, std::vector<std::string> parameters) { ChatUnBlackList(player, parameters); });
}

void AdminPlayersPlugin::Init()
{

}

void AdminPlayersPlugin::ChatKick(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() >= 1)
    {
        std::string login = parameters.at(0);
        if(controller->Players->find(login) != controller->Players->end())
        {
            Player kickPlayer = controller->Players->at(login);

            if(parameters.size() > 1)
            {
                std::stringstream message;
                for(int paramId = 1; paramId < parameters.size(); paramId++)
                {
                    if(paramId > 1) message << " ";
                    message << parameters.at(paramId);
                }
                controller->Server->Kick(login, message.str());
            }
            else
            {
                controller->Server->Kick(login);
            }

            GbxError error = controller->Server->GetCurrentError();
            if(error.number == 0 && error.message.empty())
            {
                std::stringstream chatMessage;
                chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 kicked $fff" << kickPlayer.NickName << "$z$s$ff0 from the server.";
                controller->Server->ChatSendServerMessage(chatMessage.str());
            }
            else
            {
                std::stringstream chatMessage;
                chatMessage << "$f00$iError: " << error.message << " (" << error.number << ").";
                controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
            }
        }
        else
        {
            std::stringstream chatMessage;
            chatMessage << "$f00$iUnknown player '$fff" << login << "$f00'!";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
        }
    }
    else
    {
        controller->Server->ChatSendServerMessageToLogin("$f00$iNeed to provide a player login to kick!", player.Login);
    }
}

void AdminPlayersPlugin::ChatBan(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() >= 1)
    {
        std::string login = parameters.at(0);
        if(controller->Players->find(login) != controller->Players->end())
        {
            Player banPlayer = controller->Players->at(login);

            if(parameters.size() > 1)
            {
                std::stringstream message;
                for(int paramId = 1; paramId < parameters.size(); paramId++)
                {
                    if(paramId > 1) message << " ";
                    message << parameters.at(paramId);
                }
                controller->Server->Ban(login, message.str());
            }
            else
            {
                controller->Server->Ban(login);
            }

            GbxError error = controller->Server->GetCurrentError();
            if(error.number == 0 && error.message.empty())
            {
                std::stringstream chatMessage;
                chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 banned $fff" << banPlayer.NickName << "$z$s$ff0 from the server.";
                controller->Server->ChatSendServerMessage(chatMessage.str());
            }
            else
            {
                std::stringstream chatMessage;
                chatMessage << "$f00$iError: " << error.message << " (" << error.number << ").";
                controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
            }
        }
        else
        {
            std::stringstream chatMessage;
            chatMessage << "$f00$iUnknown player '$fff" << login << "$f00'!";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
        }
    }
    else
    {
        controller->Server->ChatSendServerMessageToLogin("$f00$iNeed to provide a player login to ban!", player.Login);
    }
}

void AdminPlayersPlugin::ChatUnban(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() >= 1)
    {
        std::string login = parameters.at(0);
        controller->Server->UnBan(login);

        GbxError error = controller->Server->GetCurrentError();
        if(error.number == 0 && error.message.empty())
        {
            std::stringstream chatMessage;
            chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 unbanned $fff" << login << "$z$s$ff0 from the server.";
            controller->Server->ChatSendServerMessage(chatMessage.str());
        }
        else
        {
            std::stringstream chatMessage;
            chatMessage << "$f00$iError: " << error.message << " (" << error.number << ").";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
        }
    }
    else
    {
        controller->Server->ChatSendServerMessageToLogin("$f00$iNeed to provide a player login to unban!", player.Login);
    }
}

void AdminPlayersPlugin::ChatBlackList(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() >= 1)
    {
        std::string login = parameters.at(0);
        controller->Server->BlackList(login);

        if(controller->Players->find(login) != controller->Players->end())
        {
            Player blackPlayer = controller->Players->at(login);
            if(parameters.size() > 1)
            {
                std::stringstream message;
                for(int paramId = 1; paramId < parameters.size(); paramId++)
                {
                    if(paramId > 1) message << " ";
                    message << parameters.at(paramId);
                }
                controller->Server->Ban(login, message.str());
            }
            else
            {
                controller->Server->Ban(login);
            }

            GbxError error = controller->Server->GetCurrentError();
            if(error.number == 0 && error.message.empty())
            {
                std::stringstream chatMessage;
                chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 banned and blacklisted $fff" << blackPlayer.NickName << "$z$s$ff0 from the server.";
                controller->Server->ChatSendServerMessage(chatMessage.str());
            }
            else
            {
                std::stringstream chatMessage;
                chatMessage << "$f00$iError: " << error.message << " (" << error.number << ").";
                controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
            }
        }
        else
        {
            GbxError error = controller->Server->GetCurrentError();
            if(error.number == 0 && error.message.empty())
            {
                std::stringstream chatMessage;
                chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 blacklisted $fff" << login << "$z$s$ff0 from the server.";
                controller->Server->ChatSendServerMessage(chatMessage.str());
            }
            else
            {
                std::stringstream chatMessage;
                chatMessage << "$f00$iError: " << error.message << " (" << error.number << ").";
                controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
            }
        }
    }
    else
    {
        controller->Server->ChatSendServerMessageToLogin("$f00$iNeed to provide a player login to blacklist!", player.Login);
    }
}

void AdminPlayersPlugin::ChatUnBlackList(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() >= 1)
    {
        std::string login = parameters.at(0);
        controller->Server->UnBlackList(login);

        GbxError error = controller->Server->GetCurrentError();
        if(error.number == 0 && error.message.empty())
        {
            std::stringstream chatMessage;
            chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 unblacklisted $fff" << login << "$z$s$ff0 from the server.";
            controller->Server->ChatSendServerMessage(chatMessage.str());
        }
        else
        {
            std::stringstream chatMessage;
            chatMessage << "$f00$iError: " << error.message << " (" << error.number << ").";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
        }
    }
    else
    {
        controller->Server->ChatSendServerMessageToLogin("$f00$iNeed to provide a player login to unblacklist!", player.Login);
    }
}
