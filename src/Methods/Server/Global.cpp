#include "../Methods.h"

bool Methods::Authenticate(std::string username, std::string password)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&username);
    params->Put(&password);

    GbxMessage* message = new GbxMessage("Authenticate", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

bool Methods::ChangeAuthPassword(std::string username, std::string password)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&username);
    params->Put(&password);

    GbxMessage* message = new GbxMessage("ChangeAuthPassword", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

bool Methods::EnableCallbacks(bool enable)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&enable);

    GbxMessage* message = new GbxMessage("EnableCallbacks", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

bool Methods::SetApiVersion(std::string version)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&version);

    GbxMessage* message = new GbxMessage("SetApiVersion", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}
