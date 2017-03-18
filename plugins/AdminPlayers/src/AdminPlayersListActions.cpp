#include "AdminPlayersPlugin.h"

void AdminPlayersPlugin::ChatClearBanList(Player player)
{
    controller->Server->CleanBanList();

    std::stringstream chatMessage;
    chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 cleared the server banlist.";
    controller->Server->ChatSendServerMessage(chatMessage.str());
}

void AdminPlayersPlugin::ChatClearBlackList(Player player)
{
    controller->Server->CleanBlackList();

    std::stringstream chatMessage;
    chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 cleared the server blacklist.";
    controller->Server->ChatSendServerMessage(chatMessage.str());
}

void AdminPlayersPlugin::ChatLoadBlackList(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() > 0)
    {
        std::string file = parameters.at(0);
        controller->Server->LoadBlackList(file);

        GbxError error = controller->Server->GetCurrentError();
        if(error.number == 0 && error.message.empty())
        {
            std::stringstream chatMessage;
            chatMessage << "$ff0Black list $fff" << file << "$ff0 has been loaded.";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
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
        chatMessage << "$f00$iError: no filename supplied!";
        controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
    }
}

void AdminPlayersPlugin::ChatSaveBlackList(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() > 0)
    {
        std::string file = parameters.at(0);
        controller->Server->SaveBlackList(file);

        GbxError error = controller->Server->GetCurrentError();
        if(error.number == 0 && error.message.empty())
        {
            std::stringstream chatMessage;
            chatMessage << "$ff0Black list $fff" << file << "$ff0 has been saved.";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
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
        chatMessage << "$f00$iError: no filename supplied!";
        controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
    }
}

void AdminPlayersPlugin::ChatLoadGuestList(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() > 0)
    {
        std::string file = parameters.at(0);
        controller->Server->LoadGuestList(file);

        GbxError error = controller->Server->GetCurrentError();
        if(error.number == 0 && error.message.empty())
        {
            std::stringstream chatMessage;
            chatMessage << "$Guest list $fff" << file << "$ff0 has been loaded.";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
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
        chatMessage << "$f00$iError: no filename supplied!";
        controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
    }
}

void AdminPlayersPlugin::ChatSaveGuestList(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() > 0)
    {
        std::string file = parameters.at(0);
        controller->Server->SaveGuestList(file);

        GbxError error = controller->Server->GetCurrentError();
        if(error.number == 0 && error.message.empty())
        {
            std::stringstream chatMessage;
            chatMessage << "$ff0Guest list $fff" << file << "$ff0 has been saved.";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
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
        chatMessage << "$f00$iError: no filename supplied!";
        controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
    }
}

void AdminPlayersPlugin::ChatClearGuestList(Player player)
{
    controller->Server->CleanGuestList();

    std::stringstream chatMessage;
    chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 cleared the server guestlist.";
    controller->Server->ChatSendServerMessage(chatMessage.str());
}

void AdminPlayersPlugin::ChatClearIgnoreList(Player player)
{
    controller->Server->CleanIgnoreList();

    std::stringstream chatMessage;
    chatMessage << "$ff0" << Permissions::Text(player.AccessLevel) << " $fff" << player.NickName << "$z$s$ff0 cleared the server ignorelist.";
    controller->Server->ChatSendServerMessage(chatMessage.str());
}
