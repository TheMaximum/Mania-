#include "../Methods.h"

std::string Methods::GameDataDirectory()
{
    std::string response;

    if(server->Query(GbxMessage("GameDataDirectory")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}

std::string Methods::GetMapsDirectory()
{
    std::string response;

    if(server->Query(GbxMessage("GetMapsDirectory")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}

std::string Methods::GetSkinsDirectory()
{
    std::string response;

    if(server->Query(GbxMessage("GetSkinsDirectory")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}
