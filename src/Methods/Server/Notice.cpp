#include "../Methods.h"

bool Methods::SendNotice(std::string text, std::string avatarLogin, int variant)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&text);
    params->Put(&avatarLogin);
    params->Put(&variant);

    GbxMessage* message = new GbxMessage("SendNotice", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

bool Methods::SendNoticeToLogin(std::string login, std::string text, std::string avatarLogin, int variant)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&login);
    params->Put(&text);
    params->Put(&avatarLogin);
    params->Put(&variant);

    GbxMessage* message = new GbxMessage("SendNoticeToLogin", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}
