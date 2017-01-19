#include "../Methods.h"

bool Methods::SetGameMode(GameMode mode)
{
    bool response = false;
    int gameMode = (int)static_cast<std::underlying_type<GameMode>::type>(mode);

    GbxParameters params = GbxParameters();
    params.Put(&gameMode);

    if(server->Query(GbxMessage("SetGameMode", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

GameMode Methods::GetGameMode()
{
    GameMode response = GameMode::Script;

    if(server->Query(GbxMessage("GetGameMode")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = static_cast<GameMode>(atoi(responseParams.at(0).GetString().c_str()));
    }

    return response;
}
