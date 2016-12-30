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

std::vector<std::string> Methods::ListMethods()
{
    std::vector<std::string> methods = std::vector<std::string>();
    GbxMessage* message = new GbxMessage("system.listMethods");
    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> methodsArray = responseParams.at(0).GetArray();

        for(int methodId = 0; methodId < methodsArray.size(); methodId++)
        {
            methods.push_back(methodsArray.at(methodId).GetString());
        }
    }

    delete message; message = NULL;
    return methods;
}

ServerVersion Methods::GetVersion()
{
    ServerVersion serverVersion = ServerVersion();

    GbxMessage* message = new GbxMessage("GetVersion");
    if(server->Query(message))
    {
        delete message; message = NULL;

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

    GbxMessage* message = new GbxMessage("GetSystemInfo");
    if(server->Query(message))
    {
        delete message; message = NULL;

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
