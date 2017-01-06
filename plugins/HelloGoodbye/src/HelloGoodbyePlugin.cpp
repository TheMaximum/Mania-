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
    chatMessage << "$39fPlayer joins: ";
    chatMessage << player.NickName;
    chatMessage << " $z$s$39fNation: $fff";
    chatMessage << player.Country;
    chatMessage << " $z$s$39fLadder: $fff";
    chatMessage << player.LadderRanking;

    controller->Server->ChatSendServerMessage(chatMessage.str());
}

void HelloGoodbyePlugin::OnPlayerDisconnect(Player player)
{
    std::cout << "PLUGIN Player Disconnected: " << player.Login << "!" << std::endl;

    std::stringstream chatMessage;
    chatMessage << player.NickName;
    chatMessage << " $z$s$39fhas left the game.";

    controller->Server->ChatSendServerMessage(chatMessage.str());
}
