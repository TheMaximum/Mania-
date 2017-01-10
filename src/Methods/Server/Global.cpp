#include "../Methods.h"

bool Methods::Authenticate(std::string username, std::string password)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&username);
    params.Put(&password);

    if(server->Query(GbxMessage("Authenticate", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::ChangeAuthPassword(std::string username, std::string password)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&username);
    params.Put(&password);

    if(server->Query(GbxMessage("ChangeAuthPassword", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::EnableCallbacks(bool enable)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&enable);

    if(server->Query(GbxMessage("EnableCallbacks", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

bool Methods::SetApiVersion(std::string version)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&version);

    if(server->Query(GbxMessage("SetApiVersion", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<std::string> Methods::ListMethods()
{
    std::vector<std::string> methods = std::vector<std::string>();

    if(server->Query(GbxMessage("system.listMethods")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> methodsArray = responseParams.at(0).GetArray();

        for(int methodId = 0; methodId < methodsArray.size(); methodId++)
        {
            methods.push_back(methodsArray.at(methodId).GetString());
        }
    }

    return methods;
}

ServerVersion Methods::GetVersion()
{
    ServerVersion serverVersion = ServerVersion();

    if(server->Query(GbxMessage("GetVersion")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> versionStruct = responseParams.at(0).GetStruct();

        serverVersion.ApiVersion = versionStruct.find("ApiVersion")->second.GetString();
        serverVersion.Build = versionStruct.find("Build")->second.GetString();
        serverVersion.Name = versionStruct.find("Name")->second.GetString();
        serverVersion.TitleId = versionStruct.find("TitleId")->second.GetString();
        serverVersion.Version = versionStruct.find("Version")->second.GetString();
    }

    return serverVersion;
}

SystemInfo Methods::GetSystemInfo()
{
    SystemInfo systemInfo = SystemInfo();

    if(server->Query(GbxMessage("GetSystemInfo")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> infoStruct = responseParams.at(0).GetStruct();

        systemInfo.ConnectionDownloadRate = atoi(infoStruct.find("ConnectionDownloadRate")->second.GetString().c_str());
        systemInfo.ConnectionUploadRate = atoi(infoStruct.find("ConnectionUploadRate")->second.GetString().c_str());
        std::istringstream(infoStruct.find("IsDedicated")->second.GetString()) >> systemInfo.IsDedicated;
        std::istringstream(infoStruct.find("IsServer")->second.GetString()) >> systemInfo.IsServer;
        systemInfo.P2PPort = atoi(infoStruct.find("P2PPort")->second.GetString().c_str());
        systemInfo.Port = atoi(infoStruct.find("Port")->second.GetString().c_str());
        systemInfo.PublishedIp = infoStruct.find("PublishedIp")->second.GetString();
        systemInfo.ServerLogin = infoStruct.find("ServerLogin")->second.GetString();
        systemInfo.ServerPlayerId = atoi(infoStruct.find("ServerPlayerId")->second.GetString().c_str());
        systemInfo.TitleId = infoStruct.find("TitleId")->second.GetString();
    }

    return systemInfo;
}

bool Methods::SetConnectionRates(int downloadRate, int uploadRate)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&downloadRate);
    params.Put(&uploadRate);

    if(server->Query(GbxMessage("SetConnectionRates", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

ServerStatus Methods::GetStatus()
{
    ServerStatus serverStatus = ServerStatus();

    if(server->Query(GbxMessage("GetStatus")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> statusStruct = responseParams.at(0).GetStruct();

        serverStatus.Code = atoi(statusStruct.find("Code")->second.GetString().c_str());
        serverStatus.Name = statusStruct.find("Name")->second.GetString();
    }

    return serverStatus;
}

bool Methods::QuitGame()
{
    bool response = false;

    if(server->Query(GbxMessage("QuitGame")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}
