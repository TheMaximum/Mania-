#include "../Methods.h"

bool Methods::ChatSendServerMessage(std::string text)
{
    bool response = false;

    std::string sendText = "»» " + text;

    GbxParameters* params = new GbxParameters();
    params->Put(&sendText);

    GbxMessage* message = new GbxMessage("ChatSendServerMessage", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

bool Methods::ChatSendServerMessageToLogin(std::string text, std::string login)
{
    bool response = false;

    std::string sendText = "» " + text;

    GbxParameters* params = new GbxParameters();
    params->Put(&sendText);
    params->Put(&login);

    GbxMessage* message = new GbxMessage("ChatSendServerMessageToLogin", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

bool Methods::ChatSendServerMessageToPlayer(std::string text, Player player)
{
    return ChatSendServerMessageToLogin(text, player.Login);
}

std::vector<std::string> Methods::GetChatLines()
{
    std::vector<std::string> chatLines = std::vector<std::string>();
    GbxMessage* message = new GbxMessage("GetChatLines");

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters().at(0).GetArray();
        for(int paramId = 0; paramId < responseParams.size(); paramId++)
        {
            chatLines.push_back(responseParams.at(paramId).GetString());
        }
    }

    delete message; message = NULL;
    return chatLines;
}

bool Methods::ChatEnableManualRouting(bool enable, bool autoForward)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&enable);
    params->Put(&autoForward);

    GbxMessage* message = new GbxMessage("ChatEnableManualRouting", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

bool Methods::ChatForwardToLogin(std::string text, std::string sender, std::string destination)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&text);
    params->Put(&sender);
    if(!destination.empty())
        params->Put(&destination);

    GbxMessage* message = new GbxMessage("ChatForwardToLogin", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}
