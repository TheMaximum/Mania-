#include "HelloGoodbyePlugin.h"

HelloGoodbyePlugin::HelloGoodbyePlugin()
{
    MethodsPlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    MethodsPlayerDisconnect.push_back([this](Player player) { OnPlayerDisconnect(player); });
}

void HelloGoodbyePlugin::Init()
{
    std::cout << "[  INFO   ] Current amount of maps: " << maps->size() << std::endl;
}

void HelloGoodbyePlugin::OnPlayerConnect(Player player)
{
    std::cout << "PLUGIN Player Connected: " << player.Login << "!" << std::endl;

    GbxParameters* params = new GbxParameters();
    std::string login = player.Login;
    params->Put(&login);

    GbxMessage* message = new GbxMessage("GetDetailedPlayerInfo", params);
    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::cout << "PLUGIN Response: " << responseParams.at(0).Type << std::endl;
    }
}

void HelloGoodbyePlugin::OnPlayerDisconnect(Player player)
{
    std::cout << "PLUGIN Player Disconnected: " << player.Login << "!" << std::endl;
}
