#include "HelloGoodbyePlugin.h"

HelloGoodbyePlugin::HelloGoodbyePlugin()
{
    Version = "0.1.0";
    Author = "TheM";
    Description = "Welcomes and says goodbye to joining and leaving players.";

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
    chatMessage << "$39fPlayer joins: $fff";
    chatMessage << Formatting::StripColors(player.NickName);
    chatMessage << " $39fNation: $fff";
    chatMessage << player.Country;
    chatMessage << " $39fLadder: $fff";
    chatMessage << player.LadderRanking;

    controller->Server->ChatSendServerMessage(chatMessage.str());
}

void HelloGoodbyePlugin::OnPlayerDisconnect(Player player)
{
    std::cout << "PLUGIN Player Disconnected: " << player.Login << "!" << std::endl;

    time_t timeOnline = std::time(0) - player.JoinedAt; // Time online in seconds

    std::stringstream chatMessage;
    chatMessage << "$fff" << Formatting::StripColors(player.NickName);
    chatMessage << " $39fhas left the game, time played: $fff" << Time::FormatSeconds(timeOnline) << "$39f.";

    controller->Server->ChatSendServerMessage(chatMessage.str());
}
