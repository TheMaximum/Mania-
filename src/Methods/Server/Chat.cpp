#include "../Methods.h"

bool Methods::ChatSendServerMessage(std::string text)
{
    bool response = false;

    std::string sendText = "»» " + text;

    GbxParameters params = GbxParameters();
    params.Put(&sendText);

    if(server->Query(GbxMessage("ChatSendServerMessage", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::ChatSendServerMessageToLogin(std::string text, std::string login)
{
    bool response = false;

    std::string sendText = "» " + text;

    GbxParameters params = GbxParameters();
    params.Put(&sendText);
    params.Put(&login);

    if(server->Query(GbxMessage("ChatSendServerMessageToLogin", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::ChatSendServerMessageToPlayer(std::string text, Player player)
{
    return ChatSendServerMessageToLogin(text, player.Login);
}

std::vector<std::string> Methods::GetChatLines()
{
    std::vector<std::string> chatLines = std::vector<std::string>();

    if(server->Query(GbxMessage("GetChatLines")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters().at(0).GetArray();
        for(int paramId = 0; paramId < responseParams.size(); paramId++)
        {
            chatLines.push_back(responseParams.at(paramId).GetString());
        }
    }

    return chatLines;
}

bool Methods::ChatEnableManualRouting(bool enable, bool autoForward)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&enable);
    params.Put(&autoForward);

    if(server->Query(GbxMessage("ChatEnableManualRouting", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::ChatForwardToLogin(std::string text, std::string sender, std::string destination)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&text);
    params.Put(&sender);
    if(!destination.empty())
        params.Put(&destination);

    if(server->Query(GbxMessage("ChatForwardToLogin", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}
