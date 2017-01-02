#include "../Methods.h"

bool Methods::CallVote(std::string xmlRequest)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&xmlRequest);

    if(server->Query(GbxMessage("CallVote", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::CallVoteEx(std::string xmlRequest, double ratio, int timeout, int whoVotes)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&xmlRequest);
    params.Put(&ratio);
    params.Put(&timeout);
    params.Put(&whoVotes);

    if(server->Query(GbxMessage("CallVoteEx", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::CancelVote()
{
    bool response = false;

    if(server->Query(GbxMessage("CancelVote")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

CurrentCallVote Methods::GetCurrentCallVote()
{
    CurrentCallVote response = CurrentCallVote();

    if(server->Query(GbxMessage("GetCurrentCallVote")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> currentStruct = responseParams.at(0).GetStruct();
        response.CallerLogin = currentStruct.find("CallerLogin")->second.GetString();
        response.CmdName = currentStruct.find("CmdName")->second.GetString();
        response.CmdParam = currentStruct.find("CmdParam")->second.GetString();
    }

    return response;
}

bool Methods::SetCallVoteTimeOut(int timeout)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&timeout);

    if(server->Query(GbxMessage("SetCallVoteTimeOut", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

CurrentNextValue Methods::GetCallVoteTimeOut()
{
    CurrentNextValue response = CurrentNextValue();

    if(server->Query(GbxMessage("GetCallVoteTimeOut")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> currentStruct = responseParams.at(0).GetStruct();
        response.CurrentValue = atoi(currentStruct.find("CurrentValue")->second.GetString().c_str());
        response.NextValue = atoi(currentStruct.find("NextValue")->second.GetString().c_str());
    }

    return response;
}

bool Methods::SetCallVoteRatio(double ratio)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&ratio);

    if(server->Query(GbxMessage("SetCallVoteRatio", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

double Methods::GetCallVoteRatio()
{
    double response;

    if(server->Query(GbxMessage("GetCallVoteRatio")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = atof(responseParams.at(0).GetString().c_str());
    }

    return response;
}

bool Methods::SetCallVoteRatios(std::vector<CallVoteRatio> ratios)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&ratios);

    if(server->Query(GbxMessage("SetCallVoteRatios", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<CallVoteRatio> Methods::GetCallVoteRatios()
{
    std::vector<CallVoteRatio> response = std::vector<CallVoteRatio>();

    if(server->Query(GbxMessage("GetCallVoteRatios")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> responseArray = responseParams.at(0).GetArray();

        for(int responseId = 0; responseId < responseArray.size(); responseId++)
        {
            std::map<std::string, GbxResponseParameter> ratioStruct = responseArray.at(responseId).GetStruct();

            CallVoteRatio ratio = CallVoteRatio();
            ratio.Command = ratioStruct.find("Command")->second.GetString();
            ratio.Ratio = atof(ratioStruct.find("Ratio")->second.GetString().c_str());
            response.push_back(ratio);
        }
    }

    return response;
}

bool Methods::SetCallVoteRatiosEx(bool replaceAll, std::vector<ExtendedCallVoteRatio> ratios)
{
    return false;

    /*bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&replaceAll);
    //params.Put(&ratios);

    std::vector<std::map<std::string, GbxR

    GbxMessage message = GbxMessage("SetCallVoteRatiosEx", params);
    std::cout << std::endl << "CallVote ratios: " << message.GetXml() << std::endl;

    /*if(server->Query(GbxMessage("SetCallVoteRatiosEx", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }*

    return response;*/
}

std::vector<ExtendedCallVoteRatio> Methods::GetCallVoteRatiosEx()
{
    std::vector<ExtendedCallVoteRatio> response = std::vector<ExtendedCallVoteRatio>();

    if(server->Query(GbxMessage("GetCallVoteRatiosEx")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> responseArray = responseParams.at(0).GetArray();

        for(int responseId = 0; responseId < responseArray.size(); responseId++)
        {
            std::map<std::string, GbxResponseParameter> ratioStruct = responseArray.at(responseId).GetStruct();

            ExtendedCallVoteRatio ratio = ExtendedCallVoteRatio();
            ratio.Command = ratioStruct.find("Command")->second.GetString();
            ratio.Param = ratioStruct.find("Param")->second.GetString();
            ratio.Ratio = atof(ratioStruct.find("Ratio")->second.GetString().c_str());
            response.push_back(ratio);
        }
    }

    return response;
}
