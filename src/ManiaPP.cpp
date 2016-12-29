#include "ManiaPP.h"

ManiaPP::ManiaPP()
{
    std::cout << "## Running Mania++ v" << VERSION << " #####################################################" << std::endl;

    config = new Config("config.yaml");
    logging = new Logging();
    server = new GbxRemote();
    players = new std::map<std::string, Player>();
    maps = new std::map<std::string, Map>();

    events = new EventManager();
    plugins = new PluginManager(logging, server, players, maps);
    plugins->SetEventManager(events);
}

ManiaPP::~ManiaPP()
{
    delete config; config = NULL;
    delete logging; logging = NULL;
    delete server; server = NULL;
    delete players; players = NULL;
    delete maps; maps = NULL;

    delete plugins; plugins = NULL;
    delete events; events = NULL;
}

bool ManiaPP::ConnectToServer()
{
    std::cout << "[         ] Connecting with server on " << config->Server->address << ":" << config->Server->port << " ... " << '\r' << std::flush;
    if(server->InitWithIp(config->Server->address, config->Server->port))
    {
        logging->PrintOKFlush();
        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Current protocol: GBXRemote " << server->GetProtocol() << std::endl;

        std::cout << "[         ] Authenticating on the server with user '" << config->Server->username << "' ... " << '\r' << std::flush;
        GbxParameters* params = new GbxParameters();
        params->Put(&config->Server->username);
        params->Put(&config->Server->password);

        GbxMessage* message = new GbxMessage("Authenticate", params);

        if(server->Query(message))
        {
            logging->PrintOKFlush();

            delete params; params = NULL;
            delete message; message = NULL;
            params = new GbxParameters();
            std::string apiVersion = server->GetApiVersion();
            params->Put(&apiVersion);

            std::cout << "[         ] Setting API version to '" << apiVersion << "' ... " << '\r' << std::flush;
            message = new GbxMessage("SetApiVersion", params);
            if(server->Query(message))
            {
                delete params; params = NULL;
                delete message; message = NULL;
                logging->PrintOKFlush();

                std::cout << "[         ] Retrieving server methods ... " << '\r' << std::flush;
                message = new GbxMessage("system.listMethods");
                if(server->Query(message))
                {
                    std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
                    std::vector<GbxResponseParameter> methodsArray = responseParams.at(0).GetArray();
                    std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved server methods: " << methodsArray.size() << " found." << std::endl;
                    delete message; message = NULL;

                    std::cout << "[         ] Retrieving server version ... " << '\r' << std::flush;
                    GbxMessage* message = new GbxMessage("GetVersion");
                    if(server->Query(message))
                    {
                        delete message; message = NULL;

                        responseParams = server->GetResponse()->GetParameters();
                        std::map<std::string, GbxResponseParameter> versionStruct = responseParams.at(0).GetStruct();
                        server->Build = (std::string)versionStruct.find("Build")->second.GetString();
                        server->Platform = (std::string)versionStruct.find("Name")->second.GetString();
                        server->TitleId = (std::string)versionStruct.find("TitleId")->second.GetString();
                        server->Version = (std::string)versionStruct.find("Version")->second.GetString();

                        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved server version: '" << server->Build << "'." << std::endl;

                        std::cout << "[         ] Retrieving system info ... " << '\r' << std::flush;
                        GbxMessage* message = new GbxMessage("GetSystemInfo");
                        if(server->Query(message))
                        {
                            delete message; message = NULL;
                            responseParams = server->GetResponse()->GetParameters();
                            std::map<std::string, GbxResponseParameter> systemStruct = responseParams.at(0).GetStruct();
                            server->Login = (std::string)systemStruct.find("ServerLogin")->second.GetString();

                            std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved system info, server login: '" << server->Login << "'." << std::endl;

                            retrievePlayerList();
                            retrieveMapList();

                            bool enableCallbacks = true;
                            GbxParameters* params = new GbxParameters();
                            params->Put(&enableCallbacks);
                            message = new GbxMessage("EnableCallbacks", params);
                            server->Query(message);

                            delete params; params = NULL;
                            delete message; message = NULL;

                            plugins->LoadPlugins();

                            PrintServerInfo();

                            return true;
                        }
                    }
                }
            }
        }
    }

    logging->PrintFailedFlush();
    logging->PrintError(server->GetCurrentError());

    return false;
}

void ManiaPP::PrintServerInfo()
{
    std::cout << "###############################################################################" << std::endl;
    std::cout << "  Mania++ v" << VERSION << " running on " << config->Server->address << ":" << config->Server->port << std::endl;
    std::cout << "  Game    : " << server->Platform << " / " << server->TitleId << std::endl;
    std::cout << "  Version : " << server->Version << " / " << server->Build << std::endl;
    std::cout << "###############################################################################" << std::endl;
}

void ManiaPP::MainLoop()
{
    std::cout << "Starting our loop ..." << std::endl;
    while(true)
    {
        server->ReadCallBacks();
        std::vector<GbxCallBack> callBacks = server->GetCBResponses();
        if(callBacks.size() > 0)
        {
            for(int callBackId = 0; callBackId < callBacks.size(); callBackId++)
            {
                GbxCallBack callBack = callBacks.at(callBackId);
                std::vector<GbxResponseParameter> parameters = callBack.GetParameters();
                std::string methodName = callBack.GetMethodName();

                if(methodName.find("PlayerConnect") != std::string::npos)
                {
                    GbxParameters* params = new GbxParameters();
                    std::string login = parameters.at(0).GetString();
                    params->Put(&login);

                    GbxMessage* message = new GbxMessage("GetPlayerInfo", params);
                    server->Query(message);
                    Player newPlayer = Player(server->GetResponse()->GetParameters().at(0).GetStruct());
                    players->insert(std::pair<std::string, Player>(newPlayer.Login, newPlayer));

                    std::cout << "Player connected: " << newPlayer.Login << " (# players: " << players->size() << ")" << std::endl;

                    events->CallPlayerConnect(newPlayer);

                    delete params; params = NULL;
                    delete message; message = NULL;
                }
                else if(methodName.find("PlayerDisconnect") != std::string::npos)
                {
                    GbxParameters* params = new GbxParameters();
                    std::string login = parameters.at(0).GetString();

                    Player disconnectingPlayer = players->find(login)->second;
                    players->erase(disconnectingPlayer.Login);
                    std::cout << "Player disconnected: " << disconnectingPlayer.Login << " (# players: " << players->size() << ")" << std::endl;

                    events->CallPlayerDisconnect(disconnectingPlayer);
                }
                else
                {
                    std::cout << "CALLBACK: " << methodName << " (parameters: " << parameters.size() << ")" << std::endl;
                    for(int paramId = 0; paramId < parameters.size(); paramId++)
                    {
                        GbxResponseParameter parameter = parameters.at(paramId);
                        PrintParameter(parameter, paramId);
                    }
                }
            }

            server->ResetCBResponses();
        }
    }
}

void ManiaPP::PrintParameter(GbxResponseParameter parameter, int paramId, std::string spaces, std::string parameterName)
{
    if(parameter.Type.find("array") != std::string::npos)
    {
        std::cout << spaces << "Parameter #" << paramId << ": array" << std::endl;
        spaces += "    ";
        std::vector<GbxResponseParameter> arrayParam = parameter.GetArray();
        for(int subParamId = 0; subParamId < arrayParam.size(); subParamId++)
        {
            GbxResponseParameter arrayParameter = arrayParam.at(subParamId);
            PrintParameter(arrayParameter, subParamId, spaces);
        }
    }
    else if(parameter.Type.find("struct") != std::string::npos)
    {
        std::cout << spaces << "Parameter #" << paramId << ": struct" << std::endl;
        spaces += "    ";
        std::map<std::string, GbxResponseParameter> structParam = parameter.GetStruct();
        int subParamId = 0;
        for(std::map<std::string, GbxResponseParameter>::iterator subParam = structParam.begin(); subParam != structParam.end(); ++subParam)
        {
            PrintParameter(subParam->second, subParamId, spaces, subParam->first);
            subParamId++;
        }
    }
    else
    {
        std::cout << spaces << "Parameter #" << paramId << ": " << parameter.GetString() << " (" << parameter.Type << ")";
        if(parameterName != "")
        {
            std::cout << " (" << parameterName << ")";
        }
        std::cout << std::endl;
    }
}

void ManiaPP::retrievePlayerList()
{
    std::cout << "[         ] Retrieving current player list ... " << '\r' << std::flush;
    int playerListLimit = 512; int playerListIndex = 0;
    GbxParameters* params = new GbxParameters();
    params->Put(&playerListLimit);
    params->Put(&playerListIndex);
    GbxMessage* getPlayerList = new GbxMessage("GetPlayerList", params);
    if(server->Query(getPlayerList))
    {
        delete getPlayerList; getPlayerList = NULL;
        delete params; params = NULL;

        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> playerList = responseParams.at(0).GetArray();

        for(int playerId = 0; playerId < playerList.size(); playerId++)
        {
            std::map<std::string, GbxResponseParameter> player = playerList.at(playerId).GetStruct();
            if(player.find("Login")->second.GetString() != server->Login)
            {
                Player newPlayer = Player(player);
                players->insert(std::pair<std::string, Player>(newPlayer.Login, newPlayer));
            }
        }

        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved current player list: " << players->size() << " found." << std::endl;

        std::map<std::string, Player> playerMap = *players;
        for(std::map<std::string, Player>::iterator it = playerMap.begin(); it != playerMap.end(); ++it)
        {
            Player listPlayer = it->second;
            std::cout << "Player #" << listPlayer.PlayerId << ":" << std::endl;
            std::cout << "    Team #           : " << listPlayer.TeamId << std::endl;
            std::cout << "    Login            : " << listPlayer.Login << std::endl;
            std::cout << "    NickName         : " << listPlayer.NickName << std::endl;
            std::cout << "    SpectatorStatus  : " << listPlayer.SpectatorStatus << std::endl;
            std::cout << "    Flags            : " << listPlayer.Flags << std::endl;
            std::cout << "    LadderRanking    : " << listPlayer.LadderRanking << std::endl;
        }
    }
    else
    {
        logging->PrintFailedFlush();
        logging->PrintError(server->GetCurrentError());
    }
}

void ManiaPP::retrieveMapList()
{
    std::cout << "[         ] Retrieving current map list ... " << '\r' << std::flush;

    int mapListLimit = 2048; int mapListIndex = 0;
    GbxParameters* params = new GbxParameters();
    params->Put(&mapListLimit);
    params->Put(&mapListIndex);
    GbxMessage* getMapList = new GbxMessage("GetMapList", params);
    if(server->Query(getMapList))
    {
        delete getMapList; getMapList = NULL;
        delete params; params = NULL;

        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> mapList = responseParams.at(0).GetArray();

        for(int mapId = 0; mapId < mapList.size(); mapId++)
        {
            std::map<std::string, GbxResponseParameter> map = mapList.at(mapId).GetStruct();
            Map newMap = Map(map);
            maps->insert(std::pair<std::string, Map>(newMap.UId, newMap));
        }

        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved current map list: " << maps->size() << " found." << std::endl;
    }
    else
    {
        logging->PrintFailedFlush();
        logging->PrintError(server->GetCurrentError());
    }
}

void ManiaPP::Terminate()
{
    server->Terminate();
}
