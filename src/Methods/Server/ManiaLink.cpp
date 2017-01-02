#include "../Methods.h"

bool Methods::SendDisplayManialinkPage(std::string page, int timeout, bool hideOnClick)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&page);
    params.Put(&timeout);
    params.Put(&hideOnClick);

    if(server->Query(GbxMessage("SendDisplayManialinkPage", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SendDisplayManialinkPageToLogin(std::string login, std::string page, int timeout, bool hideOnClick)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);
    params.Put(&page);
    params.Put(&timeout);
    params.Put(&hideOnClick);

    if(server->Query(GbxMessage("SendDisplayManialinkPageToLogin", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SendHideManialinkPage()
{
    bool response = false;

    if(server->Query(GbxMessage("SendHideManialinkPage")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SendHideManialinkPageToLogin(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("SendHideManialinkPageToLogin", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<ManiaLinkPageAnswer> Methods::GetManialinkPageAnswers()
{
    std::vector<ManiaLinkPageAnswer> answers = std::vector<ManiaLinkPageAnswer>();

    if(server->Query(GbxMessage("GetManialinkPageAnswers")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters().at(0).GetArray();
        for(int paramId = 0; paramId < responseParams.size(); paramId++)
        {
            std::map<std::string, GbxResponseParameter> answer = responseParams.at(paramId).GetStruct();
            std::string login = answer.find("Login")->second.GetString();
            Player player = playerList->find(login)->second;
            int result = atoi(answer.find("Result")->second.GetString().c_str());

            ManiaLinkPageAnswer linkAnswer = ManiaLinkPageAnswer();
            linkAnswer.AnsweringPlayer = player;
            linkAnswer.Result = result;

            answers.push_back(linkAnswer);
        }
    }

    return answers;
}
