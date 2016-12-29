#include "HelloGoodbyePlugin.h"

HelloGoodbyePlugin::HelloGoodbyePlugin()
{
    MethodsPlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    MethodsPlayerDisconnect.push_back([this](Player player) { OnPlayerDisconnect(player); });
}

void HelloGoodbyePlugin::Init()
{
    std::cout << "Current amount of maps: " << maps->size() << std::endl;
}

void HelloGoodbyePlugin::OnPlayerConnect(Player player)
{
    std::cout << "PLUGIN Player Connected: " << player.Login << "!" << std::endl;
}

void HelloGoodbyePlugin::OnPlayerDisconnect(Player player)
{
    std::cout << "PLUGIN Player Disconnected: " << player.Login << "!" << std::endl;
}
