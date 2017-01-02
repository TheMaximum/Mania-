#include "../Methods.h"

bool Methods::SendNotice(std::string text, std::string avatarLogin, int variant)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&text);
    params.Put(&avatarLogin);
    params.Put(&variant);

    if(server->Query(GbxMessage("SendNotice", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SendNoticeToLogin(std::string login, std::string text, std::string avatarLogin, int variant)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);
    params.Put(&text);
    params.Put(&avatarLogin);
    params.Put(&variant);

    if(server->Query(GbxMessage("SendNoticeToLogin", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}
