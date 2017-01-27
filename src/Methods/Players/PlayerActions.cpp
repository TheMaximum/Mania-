#include "../Methods.h"

bool Methods::Kick(std::string login, std::string message)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);
    if(message != "")
        params.Put(&message);

    if(server->Query(GbxMessage("Kick", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::Ban(std::string login, std::string message)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);
    if(message != "")
        params.Put(&message);

    if(server->Query(GbxMessage("Ban", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::BanAndBlackList(std::string login, std::string message, bool save)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);
    params.Put(&message);
    params.Put(&save);

    if(server->Query(GbxMessage("BanAndBlackList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::UnBan(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("UnBan", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::CleanBanList()
{
    bool response = false;

    if(server->Query(GbxMessage("CleanBanList")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<BannedPlayer> Methods::GetBanList(int amount, int index)
{
    std::vector<BannedPlayer> response = std::vector<BannedPlayer>();

    GbxParameters params = GbxParameters();
    params.Put(&amount);
    params.Put(&index);

    if(server->Query(GbxMessage("GetBanList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> list = responseParams.at(0).GetArray();

        for(int itemId = 0; itemId < list.size(); itemId++)
        {
            std::map<std::string, GbxResponseParameter> item = list.at(itemId).GetStruct();
            BannedPlayer newItem = BannedPlayer();
            newItem.Login = item.find("Login")->second.GetString();
            newItem.ClientName = item.find("ClientName")->second.GetString();
            newItem.IPAddress = item.find("IPAddress")->second.GetString();
            response.push_back(newItem);
        }
    }

    return response;
}

bool Methods::BlackList(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("BlackList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::UnBlackList(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("UnBlackList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::CleanBlackList()
{
    bool response = false;

    if(server->Query(GbxMessage("CleanBlackList")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<std::string> Methods::GetBlackList(int amount, int index)
{
    std::vector<std::string> response = std::vector<std::string>();

    GbxParameters params = GbxParameters();
    params.Put(&amount);
    params.Put(&index);

    if(server->Query(GbxMessage("GetBlackList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> list = responseParams.at(0).GetArray();

        for(int itemId = 0; itemId < list.size(); itemId++)
        {
            std::map<std::string, GbxResponseParameter> item = list.at(itemId).GetStruct();
            response.push_back(item.find("Login")->second.GetString());
        }
    }

    return response;
}

bool Methods::LoadBlackList(std::string file)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&file);

    if(server->Query(GbxMessage("LoadBlackList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SaveBlackList(std::string file)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&file);

    if(server->Query(GbxMessage("SaveBlackList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::AddGuest(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("AddGuest", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::RemoveGuest(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("RemoveGuest", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::CleanGuestList()
{
    bool response = false;

    if(server->Query(GbxMessage("CleanGuestList")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<std::string> Methods::GetGuestList(int amount, int index)
{
    std::vector<std::string> response = std::vector<std::string>();

    GbxParameters params = GbxParameters();
    params.Put(&amount);
    params.Put(&index);

    if(server->Query(GbxMessage("GetGuestList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> list = responseParams.at(0).GetArray();

        for(int itemId = 0; itemId < list.size(); itemId++)
        {
            std::map<std::string, GbxResponseParameter> item = list.at(itemId).GetStruct();
            response.push_back(item.find("Login")->second.GetString());
        }
    }

    return response;
}

bool Methods::LoadGuestList(std::string file)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&file);

    if(server->Query(GbxMessage("LoadGuestList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SaveGuestList(std::string file)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&file);

    if(server->Query(GbxMessage("SaveGuestList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::Ignore(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("Ignore", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::UnIgnore(std::string login)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("UnIgnore", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::CleanIgnoreList()
{
    bool response = false;

    if(server->Query(GbxMessage("CleanIgnoreList")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<std::string> Methods::GetIgnoreList(int amount, int index)
{
    std::vector<std::string> response = std::vector<std::string>();

    GbxParameters params = GbxParameters();
    params.Put(&amount);
    params.Put(&index);

    if(server->Query(GbxMessage("GetIgnoreList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> list = responseParams.at(0).GetArray();

        for(int itemId = 0; itemId < list.size(); itemId++)
        {
            std::map<std::string, GbxResponseParameter> item = list.at(itemId).GetStruct();
            response.push_back(item.find("Login")->second.GetString());
        }
    }

    return response;
}
