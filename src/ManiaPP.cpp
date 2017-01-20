#include "ManiaPP.h"

ManiaPP::ManiaPP()
{
    std::cout << "## Running Mania++ v" << VERSION << " #####################################################" << std::endl;

    config = new Config("config.yaml");

    if(config->Program->checkVersion)
    {
        VersionChecker versionChecker = VersionChecker();
        versionChecker.CheckForUpdates("TheMaximum/mania-pp", VERSION);
    }

    logging = new Logging();
    server = new GbxRemote();
    players = new std::map<std::string, Player>();
    maps = new MapList();
    events = new EventManager();
    commands = new CommandManager();

    serverInfo = new ServerInfo();
    serverInfo->ControllerVersion = VERSION;

    methods = new Methods(server, players, serverInfo);
    ui = new UIManager(methods, events, players);
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
    delete commands; commands = NULL;
    delete callbacks; callbacks = NULL;
    delete methods; methods = NULL;

    delete ui; ui = NULL;

    if(database != NULL)
    {
        database->close();
        delete database; database = NULL;
    }
    delete db; db = NULL;

    delete serverInfo; serverInfo = NULL;
}

bool ManiaPP::ConnectToServer()
{
    std::cout << "[          ] Connecting with server on " << config->Server->address << ":" << config->Server->port << " ... " << '\r' << std::flush;
    if(server->InitWithIp(config->Server->address, config->Server->port))
    {
        logging->PrintOKFlush();
        std::cout << "[    \033[0;32mOK\033[0;0m    ] Current protocol: GBXRemote " << server->GetProtocol() << std::endl;

        std::cout << "[          ] Authenticating on the server with user '" << config->Server->username << "' ... " << '\r' << std::flush;
        if(methods->Authenticate(config->Server->username, config->Server->password))
        {
            logging->PrintOKFlush();

            std::string apiVersion = server->GetApiVersion();
            std::cout << "[          ] Setting API version to '" << apiVersion << "' ... " << '\r' << std::flush;
            if(methods->SetApiVersion(apiVersion))
            {
                logging->PrintOKFlush();

                std::cout << "[          ] Retrieving server methods ... " << '\r' << std::flush;
                std::vector<std::string> serverMethods = methods->ListMethods();
                if(serverMethods.size() > 0)
                {
                    std::cout << "[    \033[0;32mOK\033[0;0m    ] Retrieved server methods: " << serverMethods.size() << " found." << std::endl;

                    std::cout << "[          ] Retrieving server version ... " << '\r' << std::flush;
                    ServerVersion getServerVersion = methods->GetVersion();
                    if(!getServerVersion.ApiVersion.empty())
                    {
                        serverInfo->Version = getServerVersion;

                        std::cout << "[    \033[0;32mOK\033[0;0m    ] Retrieved server version: '" << serverInfo->Version.Build << "'." << std::endl;

                        std::cout << "[          ] Retrieving system info ... " << '\r' << std::flush;

                        SystemInfo getSystemInfo = methods->GetSystemInfo();
                        if(!getSystemInfo.ServerLogin.empty())
                        {
                            serverInfo->System = getSystemInfo;

                            std::cout << "[    \033[0;32mOK\033[0;0m    ] Retrieved system info, server login: '" << serverInfo->System.ServerLogin << "'." << std::endl;

                            methods->SendHideManialinkPage();

                            std::cout << "[          ] Enabling CallBacks ... " << '\r' << std::flush;
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

                                    serverInfo->Name = methods->GetServerName();
                                    serverInfo->Comment = methods->GetServerComment();
                                    serverInfo->MaxPlayers = methods->GetMaxPlayers().CurrentValue;
                                    serverInfo->MaxSpectators = methods->GetMaxSpectators().CurrentValue;
                                    serverInfo->Mode = methods->GetGameMode();

                                    plugins = new PluginManager(config, methods, commands, players, maps, database, ui, serverInfo);
                                    plugins->SetEventManager(events);
                                    callbacks = new CallBackManager(server, commands, events, database, players, maps, serverInfo);

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
    std::cout << "[          ] Connecting to the database on '" << config->Database->address << ":" << config->Database->port << "' ... " << '\r' << std::flush;
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
    std::cout << "  Name    : " << serverInfo->Name << " / " << serverInfo->Account.Login << std::endl;
    std::cout << "  Game    : " << serverInfo->Version.Name << " / " << serverInfo->Version.TitleId << " / " << GameModeConverter::GetName(serverInfo->Mode) << std::endl;
    std::cout << "  Version : " << serverInfo->Version.Version << " / " << serverInfo->Version.Build << std::endl;
    std::cout << "  Author  : TheM" << std::endl;
    std::cout << "###############################################################################" << std::endl;
}

void ManiaPP::MainLoop()
{
    time_t lastSecond = std::time(0);
    time_t lastMinute = std::time(0);

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

        time_t currentTime = std::time(0);
        if(currentTime > lastSecond)
        {
            events->CallEverySecond();
            lastSecond = currentTime;
        }

        if(currentTime >= (lastMinute + 60))
        {
            events->CallEveryMinute();
            lastMinute = currentTime;
        }
    }
}

void ManiaPP::retrievePlayerList()
{
    std::cout << "[          ] Retrieving current player list ... " << '\r' << std::flush;

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

            GbxParameters params = GbxParameters();
            std::string login = player.find("Login")->second.GetString();
            params.Put(&(login));

            GbxMessage message = GbxMessage("GetDetailedPlayerInfo", params);
            server->Query(message);
            Player newPlayer = Player(player);
            newPlayer.PlayerDetailed(server->GetResponse()->GetParameters().at(0).GetStruct());

            if(player.find("Login")->second.GetString() == serverInfo->System.ServerLogin)
            {
                serverInfo->Account = newPlayer;
            }
            else
            {
                if(database != NULL)
                {
                    sql::PreparedStatement* insertPstmt;
                    try
                    {
                        insertPstmt = database->prepareStatement("INSERT INTO `players` (`Login`, `NickName`, `Nation`, `UpdatedAt`) VALUES (?, ?, ?, ?) ON DUPLICATE KEY UPDATE `NickName` = VALUES(`NickName`), `Nation` = VALUES(`Nation`), `UpdatedAt` = VALUES(`UpdatedAt`)");
                        insertPstmt->setString(1, newPlayer.Login);
                        insertPstmt->setString(2, newPlayer.NickName);
                        insertPstmt->setString(3, newPlayer.Country);
                        insertPstmt->setString(4, Time::Current());
                        insertPstmt->executeQuery();
                    }
                    catch(sql::SQLException &e)
                    {
                        std::cout << "Failed to save database information for player '" << newPlayer.Login << "' ..." << std::endl;
                        Logging::PrintError(e.getErrorCode(), e.what());
                    }

                    if(insertPstmt != NULL)
                    {
                        delete insertPstmt;
                        insertPstmt = NULL;
                    }

                    sql::PreparedStatement* pstmt;
                    sql::ResultSet* result;
                    try
                    {
                        pstmt = database->prepareStatement("SELECT * FROM `players` WHERE `Login` = ?");
                        pstmt->setString(1, newPlayer.Login);
                        result = pstmt->executeQuery();
                        if(result->next())
                        {
                            newPlayer.SetId(result->getInt("Id"));
                        }
                    }
                    catch(sql::SQLException &e)
                    {
                        std::cout << "Failed to retrieve database information for player '" << newPlayer.Login << "' ..." << std::endl;
                        Logging::PrintError(e.getErrorCode(), e.what());
                    }

                    if(pstmt != NULL)
                    {
                        delete pstmt;
                        pstmt = NULL;
                    }

                    if(result != NULL)
                    {
                        delete result;
                        result = NULL;
                    }
                }

                players->insert(std::pair<std::string, Player>(newPlayer.Login, newPlayer));
            }
        }

        std::cout << "[    \033[0;32mOK\033[0;0m    ] Retrieved current player list: " << players->size() << " found." << std::endl;
    }
    else
    {
        logging->PrintFailedFlush();
        logging->PrintError(server->GetCurrentError());
    }
}

void ManiaPP::retrieveMapList()
{
    std::cout << "[          ] Retrieving current map list ... " << '\r' << std::flush;

    std::vector<Map> list = methods->GetMapList(2048, 0);
    if(list.size() > 0)
    {
        for(int mapId = 0; mapId < list.size(); mapId++)
        {
            Map newMap = list.at(mapId);

            if(database != NULL)
            {
                sql::PreparedStatement* insertPstmt;
                try
                {
                    insertPstmt = database->prepareStatement("INSERT IGNORE INTO `maps` (`Uid`, `Name`, `Author`, `Environment`) VALUES (?, ?, ?, ?)");
                    insertPstmt->setString(1, newMap.UId);
                    insertPstmt->setString(2, newMap.Name);
                    insertPstmt->setString(3, newMap.Author);
                    insertPstmt->setString(4, newMap.Environment);
                    insertPstmt->executeQuery();
                }
                catch(sql::SQLException &e)
                {
                    std::cout << "Failed to save database information for map '" << newMap.Name << "' ..." << std::endl;
                    Logging::PrintError(e.getErrorCode(), e.what());
                }

                if(insertPstmt != NULL)
                {
                    delete insertPstmt;
                    insertPstmt = NULL;
                }

                sql::PreparedStatement* pstmt;
                sql::ResultSet* result;
                try
                {
                    pstmt = database->prepareStatement("SELECT * FROM `maps` WHERE `UId` = ?");
                    pstmt->setString(1, newMap.UId);
                    result = pstmt->executeQuery();
                    if(result->next())
                    {
                        newMap.SetId(result->getInt("Id"));
                    }
                }
                catch(sql::SQLException &e)
                {
                    std::cout << "Failed to retrieve database information for map '" << newMap.Name << "' ..." << std::endl;
                    Logging::PrintError(e.getErrorCode(), e.what());
                }

                if(pstmt != NULL)
                {
                    delete pstmt;
                    pstmt = NULL;
                }

                if(result != NULL)
                {
                    delete result;
                    result = NULL;
                }
            }

            maps->List.insert(std::pair<std::string, Map>(newMap.UId, newMap));
        }

        std::cout << "[    \033[0;32mOK\033[0;0m    ] Retrieved current map list: " << maps->List.size() << " found." << std::endl;
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
