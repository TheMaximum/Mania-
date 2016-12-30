#include "CallBackManager.h"

CallBackManager::CallBackManager(GbxRemote* serverPtr, EventManager* eventManagerPtr, std::map<std::string, Player>* playerList, std::map<std::string, Map>* mapList)
{
    server = serverPtr;
    events = eventManagerPtr;

    players = playerList;
    maps = mapList;
}

void CallBackManager::HandleCallBack(std::string methodName, std::vector<GbxResponseParameter> parameters)
{
    if(methodName.find("ManiaPlanet.PlayerConnect") != std::string::npos)
    {
        GbxParameters* params = new GbxParameters();
        std::string login = parameters.at(0).GetString();
        params->Put(&login);

        GbxMessage* message = new GbxMessage("GetPlayerInfo", params);
        server->Query(message);
        Player newPlayer = Player(server->GetResponse()->GetParameters().at(0).GetStruct());
        players->insert(std::pair<std::string, Player>(newPlayer.Login, newPlayer));

        std::cout << "Player connected: " << newPlayer.Login << " (# players: " << players->size() << ")" << std::endl;

        events->CallPlayerConnect(newPlayer);

        delete params; params = NULL;
        delete message; message = NULL;
    }
    else if(methodName.find("ManiaPlanet.PlayerDisconnect") != std::string::npos)
    {
        std::string login = parameters.at(0).GetString();

        Player disconnectingPlayer = players->find(login)->second;
        players->erase(disconnectingPlayer.Login);
        std::cout << "Player disconnected: " << disconnectingPlayer.Login << " (# players: " << players->size() << ")" << std::endl;

        events->CallPlayerDisconnect(disconnectingPlayer);
    }
    else if(methodName.find("ManiaPlanet.PlayerChat") != std::string::npos)
    {
        std::string playerUid = parameters.at(0).GetString();
        std::string login = parameters.at(1).GetString();
        std::string answer = parameters.at(2).GetString();
        std::vector<GbxResponseParameter> entries = parameters.at(3).GetArray();



        /*Player disconnectingPlayer = players->find(login)->second;
        players->erase(disconnectingPlayer.Login);
        std::cout << "Player disconnected: " << disconnectingPlayer.Login << " (# players: " << players->size() << ")" << std::endl;

        events->CallPlayerDisconnect(disconnectingPlayer);*/
    }
    else
    {
        std::cout << "CALLBACK: " << methodName << " (parameters: " << parameters.size() << ")" << std::endl;
        for(int paramId = 0; paramId < parameters.size(); paramId++)
        {
            GbxResponseParameter parameter = parameters.at(paramId);
            Logging::PrintParameter(parameter, paramId);
        }
    }
}
