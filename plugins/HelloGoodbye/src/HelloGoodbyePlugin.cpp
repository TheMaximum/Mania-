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
    
}

void HelloGoodbyePlugin::OnPlayerConnect(Player player)
{
    std::cout << "PLUGIN Player Connected: " << player.Login << "!" << std::endl;

    std::stringstream chatMessage;
    chatMessage << "Player joins: ";
    chatMessage << player.NickName;
    chatMessage << " $s$zLadder: ";
    chatMessage << player.LadderRanking;

    controller->Server->ChatSendServerMessage(chatMessage.str());
}

void HelloGoodbyePlugin::OnPlayerDisconnect(Player player)
{
    std::cout << "PLUGIN Player Disconnected: " << player.Login << "!" << std::endl;

    std::stringstream chatMessage;
    chatMessage << player.NickName;
    chatMessage << " $s$zhas left the game.";

    controller->Server->ChatSendServerMessage(chatMessage.str());
}
