#include "HelloGoodbyePlugin.h"

HelloGoodbyePlugin::HelloGoodbyePlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    PlayerDisconnect.push_back([this](Player player) { OnPlayerDisconnect(player); });
}

void HelloGoodbyePlugin::Init()
{
    std::cout << "[  INFO   ] Current amount of maps: " << maps->size() << std::endl;
}

void HelloGoodbyePlugin::OnPlayerConnect(Player player)
{
    std::cout << "PLUGIN Player Connected: " << player.Login << "!" << std::endl;

    std::stringstream chatMessage;
    chatMessage << "Player joins: ";
    chatMessage << player.NickName;

    methods->ChatSendServerMessage(chatMessage.str());
}

void HelloGoodbyePlugin::OnPlayerDisconnect(Player player)
{
    std::cout << "PLUGIN Player Disconnected: " << player.Login << "!" << std::endl;

    std::stringstream chatMessage;
    chatMessage << "Player leaves: ";
    chatMessage << player.NickName;

    methods->ChatSendServerMessage(chatMessage.str());
}
