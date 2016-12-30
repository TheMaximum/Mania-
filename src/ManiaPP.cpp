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
    methods = new Methods(server);
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
    delete methods; methods = NULL;
}

bool ManiaPP::ConnectToServer()
{
    std::cout << "[         ] Connecting with server on " << config->Server->address << ":" << config->Server->port << " ... " << '\r' << std::flush;
    if(server->InitWithIp(config->Server->address, config->Server->port))
    {
        logging->PrintOKFlush();
        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Current protocol: GBXRemote " << server->GetProtocol() << std::endl;

        std::cout << "[         ] Authenticating on the server with user '" << config->Server->username << "' ... " << '\r' << std::flush;
        if(methods->Authenticate(config->Server->username, config->Server->password))
        {
            logging->PrintOKFlush();

            std::string apiVersion = server->GetApiVersion();
            std::cout << "[         ] Setting API version to '" << apiVersion << "' ... " << '\r' << std::flush;
            if(methods->SetApiVersion(apiVersion))
            {
                logging->PrintOKFlush();

                std::cout << "[         ] Retrieving server methods ... " << '\r' << std::flush;
                std::vector<std::string> serverMethods = methods->ListMethods();
                if(serverMethods.size() > 0)
                {
                    std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved server methods: " << serverMethods.size() << " found." << std::endl;

                    std::cout << "[         ] Retrieving server version ... " << '\r' << std::flush;
                    ServerVersion getServerVersion = methods->GetVersion();
                    if(!getServerVersion.ApiVersion.empty())
                    {
                        serverVersion = getServerVersion;

                        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved server version: '" << serverVersion.Build << "'." << std::endl;

                        std::cout << "[         ] Retrieving system info ... " << '\r' << std::flush;
                        GbxMessage* message = new GbxMessage("GetSystemInfo");
                        SystemInfo getSystemInfo = methods->GetSystemInfo();
                        if(!getSystemInfo.ServerLogin.empty())
                        {
                            systemInfo = getSystemInfo;

                            std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved system info, server login: '" << systemInfo.ServerLogin << "'." << std::endl;

                            retrievePlayerList();
                            retrieveMapList();

                            std::cout << "[         ] Enabling CallBacks ... " << '\r' << std::flush;
                            if(methods->EnableCallbacks(true))
                            {
                                logging->PrintOKFlush();

                                plugins->LoadPlugins();
                                plugins->InitializePlugins();

                                PrintServerInfo();

                                return true;
                            }
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
    std::cout << "  Game    : " << serverVersion.Name << " / " << serverVersion.TitleId << std::endl;
    std::cout << "  Version : " << serverVersion.Version << " / " << serverVersion.Build << std::endl;
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
            if(player.find("Login")->second.GetString() != systemInfo.ServerLogin)
            {
                Player newPlayer = Player(player);
                players->insert(std::pair<std::string, Player>(newPlayer.Login, newPlayer));
            }
        }

        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved current player list: " << players->size() << " found." << std::endl;
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
