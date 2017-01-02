#include "ManiaPP.h"

ManiaPP::ManiaPP()
{
    std::cout << "## Running Mania++ v" << VERSION << " #####################################################" << std::endl;

    config = new Config("config.yaml");
    logging = new Logging();
    server = new GbxRemote();
    methods = new Methods(server);
    players = new std::map<std::string, Player>();
    maps = new MapList();
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
    delete callbacks; callbacks = NULL;
    delete methods; methods = NULL;

    database->close();
    delete database; database = NULL;
    delete db; db = NULL;
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

                        SystemInfo getSystemInfo = methods->GetSystemInfo();
                        if(!getSystemInfo.ServerLogin.empty())
                        {
                            systemInfo = getSystemInfo;

                            std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved system info, server login: '" << systemInfo.ServerLogin << "'." << std::endl;

                            methods->GetChatLines();

                            std::cout << "[         ] Enabling CallBacks ... " << '\r' << std::flush;
                            if(methods->EnableCallbacks(true))
                            {
                                logging->PrintOKFlush();

                                if(ConnectToDatabase())
                                {
                                    retrievePlayerList();
                                    retrieveMapList();

                                    Map currentMap = methods->GetCurrentMapInfo();
                                    maps->SetCurrentMap(currentMap.UId);
                                    maps->Current->CopyDetailedMap(currentMap);

                                    events = new EventManager();
                                    plugins = new PluginManager(methods, players, maps, database);
                                    plugins->SetEventManager(events);
                                    callbacks = new CallBackManager(server, events, database, players, maps);

                                    plugins->LoadPlugins();
                                    plugins->InitializePlugins();

                                    PrintServerInfo();

                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
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

bool ManiaPP::ConnectToDatabase()
{
    std::cout << "[         ] Connecting to the database on '" << config->Database->address << ":" << config->Database->port << "' ... " << '\r' << std::flush;
    try
    {
        db = new Database(config->Database->address, config->Database->port);
        sql::Connection* dbConnection = db->Connect(config->Database->username, config->Database->password, config->Database->database);
        if(dbConnection != NULL)
        {
            logging->PrintOKFlush();
            database = dbConnection;
            return true;
        }
    }
    catch(sql::SQLException &e)
    {
        logging->PrintFailedFlush();
        logging->PrintError(e.getErrorCode(), e.what());
        return false;
    }

    logging->PrintFailedFlush();
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

                callbacks->HandleCallBack(methodName, parameters);
            }

            server->ResetCBResponses();
        }
    }
}

void ManiaPP::retrievePlayerList()
{
    std::cout << "[         ] Retrieving current player list ... " << '\r' << std::flush;

    int playerListLimit = 512; int playerListIndex = 0;
    GbxParameters params = GbxParameters();
    params.Put(&playerListLimit);
    params.Put(&playerListIndex);
    GbxMessage getPlayerList = GbxMessage("GetPlayerList", params);
    if(server->Query(getPlayerList))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> playerList = responseParams.at(0).GetArray();

        for(int playerId = 0; playerId < playerList.size(); playerId++)
        {
            std::map<std::string, GbxResponseParameter> player = playerList.at(playerId).GetStruct();
            if(player.find("Login")->second.GetString() != systemInfo.ServerLogin)
            {
                GbxParameters params = GbxParameters();
                std::string login = player.find("Login")->second.GetString();
                params.Put(&(login));

                GbxMessage message = GbxMessage("GetDetailedPlayerInfo", params);
                server->Query(message);
                Player newPlayer = Player(player);
                newPlayer.PlayerDetailed(server->GetResponse()->GetParameters().at(0).GetStruct());

                if(database != NULL)
                {
                    sql::PreparedStatement* pstmt;
                    pstmt = database->prepareStatement("SELECT * FROM `players` WHERE `Login` = ?");
                    pstmt->setString(1, newPlayer.Login);
                    sql::ResultSet* result = pstmt->executeQuery();
                    if(result->next())
                    {
                        newPlayer.SetId(result->getInt("Id"));
                    }

                    delete pstmt; pstmt = NULL;
                    delete result; result = NULL;
                }

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

    std::vector<Map> list = methods->GetMapList(2048, 0);
    if(list.size() > 0)
    {
        for(int mapId = 0; mapId < list.size(); mapId++)
        {
            Map newMap = list.at(mapId);

            if(database != NULL)
            {
                sql::PreparedStatement* pstmt;
                pstmt = database->prepareStatement("SELECT * FROM `maps` WHERE `UId` = ?");
                pstmt->setString(1, newMap.UId);
                sql::ResultSet* result = pstmt->executeQuery();
                if(result->next())
                {
                    newMap.SetId(result->getInt("Id"));
                }

                delete pstmt; pstmt = NULL;
                delete result; result = NULL;
            }

            maps->List.insert(std::pair<std::string, Map>(newMap.UId, newMap));
        }

        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieved current map list: " << maps->List.size() << " found." << std::endl;
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
