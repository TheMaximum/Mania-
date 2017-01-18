#include "../Methods.h"

bool Methods::SetServerName(std::string name)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&name);

    if(server->Query(GbxMessage("SetServerName", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::string Methods::GetServerName()
{
    std::string response;

    if(server->Query(GbxMessage("GetServerName")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}

bool Methods::SetServerComment(std::string comment)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&comment);

    if(server->Query(GbxMessage("SetServerComment", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::string Methods::GetServerComment()
{
    std::string response;

    if(server->Query(GbxMessage("GetServerComment")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}

bool Methods::SetHideServer(int setting)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&setting);

    if(server->Query(GbxMessage("SetHideServer", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

int Methods::GetHideServer()
{
    int response = 0;

    if(server->Query(GbxMessage("GetHideServer")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = atoi(responseParams.at(0).GetString().c_str());
    }

    return response;
}

bool Methods::IsRelayServer()
{
    bool response = false;

    if(server->Query(GbxMessage("IsRelayServer")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SetServerPassword(std::string password)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&password);

    if(server->Query(GbxMessage("SetServerPassword", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::string Methods::GetServerPassword()
{
    std::string response;

    if(server->Query(GbxMessage("GetServerPassword")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}

bool Methods::SetServerPasswordForSpectator(std::string password)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&password);

    if(server->Query(GbxMessage("SetServerPasswordForSpectator", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::string Methods::GetServerPasswordForSpectator()
{
    std::string response;

    if(server->Query(GbxMessage("GetServerPasswordForSpectator")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}

bool Methods::SetMaxPlayers(int limit)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&limit);

    if(server->Query(GbxMessage("SetMaxPlayers", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

CurrentNextValue Methods::GetMaxPlayers()
{
    CurrentNextValue response = CurrentNextValue();

    if(server->Query(GbxMessage("GetMaxPlayers")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> currentStruct = responseParams.at(0).GetStruct();
        response.CurrentValue = atoi(currentStruct.find("CurrentValue")->second.GetString().c_str());
        response.NextValue = atoi(currentStruct.find("NextValue")->second.GetString().c_str());
    }

    return response;
}

bool Methods::SetMaxSpectators(int limit)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&limit);

    if(server->Query(GbxMessage("SetMaxSpectators", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

CurrentNextValue Methods::GetMaxSpectators()
{
    CurrentNextValue response = CurrentNextValue();

    if(server->Query(GbxMessage("GetMaxSpectators")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> currentStruct = responseParams.at(0).GetStruct();
        response.CurrentValue = atoi(currentStruct.find("CurrentValue")->second.GetString().c_str());
        response.NextValue = atoi(currentStruct.find("NextValue")->second.GetString().c_str());
    }

    return response;
}
