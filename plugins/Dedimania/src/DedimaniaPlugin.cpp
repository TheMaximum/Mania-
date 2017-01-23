#include "DedimaniaPlugin.h"

DedimaniaPlugin::DedimaniaPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    PlayerFinish.push_back([this](Player player, int playerTime) { OnPlayerFinish(player, playerTime); });
    EndMatch.push_back([this](std::vector<PlayerRanking> rankings, int winnerTeam) { OnEndMatch(rankings); });

    EveryMinute.push_back([this]() {
        updateServer++;
        if(updateServer >= 3)
        {
            UpdateServer();
            updateServer = 0;
        }
    });

    RegisterCommand("dedirecs", [this](Player player, std::vector<std::string> parameters) { OpenDediRecords(player); });
    RegisterCommand("dedirecords", [this](Player player, std::vector<std::string> parameters) { OpenDediRecords(player); });

    curl_global_init(CURL_GLOBAL_ALL);
}

void DedimaniaPlugin::Init()
{
    loadSettings();
    authenticate();

    widget = DedimaniaWidget(controller->UI, &records);
    widget.WidgetEntries = widgetEntries;
    widget.WidgetTopCount = widgetTopCount;
    widget.WidgetX = widgetX;
    widget.WidgetY = widgetY;
    controller->UI->RegisterEvent("OpenDediRecords", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { OpenDediRecords(player); }));

    OnBeginMap();
}

void DedimaniaPlugin::OnBeginMap()
{
    Map currentMap = *controller->Maps->Current;
    records = std::vector<DediRecord>();
    newRecords = std::vector<DediRecord>();
    mapValid = false;

    if(sessionId != "")
    {
        updateServer = 0;

        std::string gameMode = GameModeConverter::GetDediName(controller->Info->Mode);
        if(gameMode == "n/a")
        {
            std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Not retrieving Dedimania record, as game mode is not 'TA' or 'Rounds'!" << std::endl << std::flush;
            return;
        }

        if(currentMap.NbCheckpoints < 2 && currentMap.Author != "Nadeo")
        {
            std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Not retrieving Dedimania record, number of checkpoints < 2!" << std::endl << std::flush;
            return;
        }

        if(currentMap.AuthorTime < 10000)
        {
            std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Not retrieving Dedimania record, authortime < 10s!" << std::endl << std::flush;
            return;
        }

        mapValid = true;

        std::cout << "[          ] Retrieving Dedimania records for current map ... " << std::endl << std::flush;

        int numPlayers = 0;
        int numSpecs = 0;
        bool privateServer = (controller->Server->GetServerPassword() != "");

        GbxParameters players = GbxParameters();
        std::vector<GbxStructParameters> playerVector = std::vector<GbxStructParameters>();
        for(std::map<std::string, Player>::iterator playerIt = controller->Players->begin(); playerIt != controller->Players->end(); ++playerIt)
        {
            Player player = playerIt->second;

            if(player.IsSpectator)
            {
                numSpecs++;
            }
            else
            {
                numPlayers++;
            }

            std::stringstream isSpec;
            isSpec << "<boolean>" << player.IsSpectator << "</boolean>";

            GbxStructParameters playerStruct = GbxStructParameters();
            playerStruct.Put("Login", "<string>" + player.Login + "</string>");
            playerStruct.Put("IsSpec", isSpec.str());
            playerVector.push_back(playerStruct);
        }

        for(int playerId = 0; playerId < playerVector.size(); playerId++)
        {
            players.Put(&playerVector.at(playerId));
        }

        GbxStructParameters getRecs = GbxStructParameters();
        std::string methodName = "dedimania.GetChallengeRecords";
        getRecs.Put("methodName", Parameter(&methodName));
        GbxParameters parameters = GbxParameters();
        parameters.Put(&sessionId);
            GbxStructParameters mapInfo = GbxStructParameters();
            mapInfo.Put("UId", Parameter(&currentMap.UId));
            std::string mapName = Text::EscapeXML(currentMap.Name);
            mapInfo.Put("Name", Parameter(&mapName));
            mapInfo.Put("Environment", Parameter(&currentMap.Environment));
            mapInfo.Put("Author", Parameter(&currentMap.Author));
            mapInfo.Put("NbCheckpoints", Parameter(&currentMap.NbCheckpoints));
            mapInfo.Put("NbLaps", Parameter(&currentMap.NbLaps));
        parameters.Put(&mapInfo);
        parameters.Put(&gameMode);
            GbxStructParameters serverInfo = GbxStructParameters();
            std::string srvName = Text::EscapeXML(controller->Info->Name);
            serverInfo.Put("SrvName", Parameter(&srvName));
            serverInfo.Put("Comment", Parameter(&controller->Info->Comment));
            serverInfo.Put("Private", Parameter(&privateServer));
            serverInfo.Put("NumPlayers", Parameter(&numPlayers));
            serverInfo.Put("MaxPlayers", Parameter(&controller->Info->MaxPlayers));
            serverInfo.Put("NumSpecs", Parameter(&numSpecs));
            serverInfo.Put("MaxSpecs", Parameter(&controller->Info->MaxSpectators));
        parameters.Put(&serverInfo);
        parameters.Put(&players);
        getRecs.Put("params", Parameter(&parameters));
        currentCalls.push_back(getRecs);

        GbxResponse queryResponse = multicall();
        if(!hasError)
        {
            if(queryResponse.GetParameters().at(0).GetArray().size() != 0)
            {
                std::vector<GbxResponseParameter> responseParams = queryResponse.GetParameters().at(0).GetArray().at(0).GetArray();
                std::map<std::string, GbxResponseParameter> responseStruct = responseParams.at(0).GetStruct();
                maxRank = atoi(responseStruct.find("ServerMaxRank")->second.GetString().c_str());
                std::vector<GbxResponseParameter> recordsStruct = responseStruct.find("Records")->second.GetArray();

                for(int recordId = 0; recordId < recordsStruct.size(); recordId++)
                {
                    std::map<std::string, GbxResponseParameter> recordStruct = recordsStruct.at(recordId).GetStruct();
                    DediRecord record = DediRecord(recordStruct);

                    records.push_back(record);

                    if(record.Rank == maxRank)
                        break;
                }
            }

            std::cout << "\x1b[1A[    \033[0;32mOK\033[0;0m    ] Successfully retrieved Dedimania records for current map!" << std::endl << std::flush;
        }
        else
        {
            std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Unable to retrieve records from Dedimania!" << std::endl << std::flush;
        }
    }

    if(!widget.DisplayToAll(controller->Players))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void DedimaniaPlugin::UpdateServer()
{
    Map currentMap = *controller->Maps->Current;

    if(sessionId != "")
    {
        int updateServer = 0;

        std::string gameMode = GameModeConverter::GetDediName(controller->Info->Mode);
        if(gameMode == "n/a")
            gameMode = GameModeConverter::GetName(controller->Info->Mode);
        std::cout << "[          ] Updating Dedimania with player list ... " << std::endl << std::flush;

        int numPlayers = 0;
        int numSpecs = 0;
        bool privateServer = (controller->Server->GetServerPassword() != "");

        GbxParameters players = GbxParameters();
        std::vector<GbxStructParameters> playerVector = std::vector<GbxStructParameters>();
        for(std::map<std::string, Player>::iterator playerIt = controller->Players->begin(); playerIt != controller->Players->end(); ++playerIt)
        {
            Player player = playerIt->second;

            if(player.IsSpectator)
            {
                numSpecs++;
            }
            else
            {
                numPlayers++;
            }

            std::stringstream isSpec;
            isSpec << "<boolean>" << player.IsSpectator << "</boolean>";
            std::stringstream vote;
            vote << "<int>-1</int>";

            GbxStructParameters playerStruct = GbxStructParameters();
            playerStruct.Put("Login", "<string>" + player.Login + "</string>");
            playerStruct.Put("IsSpec", isSpec.str());
            playerStruct.Put("Vote", vote.str());
            playerVector.push_back(playerStruct);
        }

        for(int playerId = 0; playerId < playerVector.size(); playerId++)
        {
            players.Put(&playerVector.at(playerId));
        }

        GbxStructParameters update = GbxStructParameters();
        std::string methodName = "dedimania.UpdateServerPlayers";
        update.Put("methodName", Parameter(&methodName));
        GbxParameters parameters = GbxParameters();
        parameters.Put(&sessionId);
        GbxStructParameters serverInfo = GbxStructParameters();
            std::string srvName = Text::EscapeXML(controller->Info->Name);
            serverInfo.Put("SrvName", Parameter(&srvName));
            serverInfo.Put("Comment", Parameter(&controller->Info->Comment));
            serverInfo.Put("Private", Parameter(&privateServer));
            serverInfo.Put("NumPlayers", Parameter(&numPlayers));
            serverInfo.Put("MaxPlayers", Parameter(&controller->Info->MaxPlayers));
            serverInfo.Put("NumSpecs", Parameter(&numSpecs));
            serverInfo.Put("MaxSpecs", Parameter(&controller->Info->MaxSpectators));
        parameters.Put(&serverInfo);
            GbxStructParameters votesInfo = GbxStructParameters();
            votesInfo.Put("UId", Parameter(&currentMap.UId));
            votesInfo.Put("GameMode", Parameter(&gameMode));
        parameters.Put(&votesInfo);
        parameters.Put(&players);
        update.Put("params", Parameter(&parameters));
        currentCalls.push_back(update);

        GbxResponse queryResponse = multicall();
        if(!hasError)
        {
            std::cout << "\x1b[1A[    \033[0;32mOK\033[0;0m    ] Successfully updated Dedimania with the player list." << std::endl << std::flush;
        }
        else
        {
            std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Unable to update Dedimania with player list." << std::endl << std::flush;
        }
    }
}

void DedimaniaPlugin::OnPlayerConnect(Player player)
{
    if(!widget.DisplayToPlayer(player))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void DedimaniaPlugin::OnPlayerFinish(Player player, int time)
{
    if(!mapValid)
        return;

    if(GameModeConverter::GetDediName(controller->Info->Mode) == "n/a")
        return;

    if(time == 0)
        return;

    if(player.CurrentCheckpoints.size() != controller->Maps->Current->NbCheckpoints ||
       time != player.CurrentCheckpoints.back())
    {
        std::cout << "[   DEDI   ] Player '" << player.Login << "' has inconsistent finish/checkpoints, ignoring: " << time << std::endl;
        return;
    }

    if(player.CurrentCheckpoints.size() < 2 && controller->Maps->Current->Author != "Nadeo")
    {
        std::cout << "[   DEDI   ] Player '" << player.Login << "' has checks < 2, ignoring: " << time << std::endl;
        return;
    }

    DediRecord playerRecord = DediRecord();
    int recordIndex = -1;
    for(int recordId = 0; recordId < records.size(); recordId++)
    {
        DediRecord record = records.at(recordId);
        if(record.Login == player.Login)
        {
            playerRecord = record;
            recordIndex = recordId;
            break;
        }
    }

    if(recordIndex == -1 || time < playerRecord.Time)
    {
        int newIndex = -1;

        for(int dediId = 0; dediId < records.size(); dediId++)
        {
            DediRecord record = records.at(dediId);
            if(time < record.Time)
            {
                newIndex = dediId;
                break;
            }
        }

        if(newIndex == -1)
        {
            if(records.size() < maxRank)
            {
                newIndex = records.size();
            }
            else
            {
                return;
            }
        }

        if(recordIndex != -1)
            records.erase((records.begin()+recordIndex));

        DediRecord newDedi = DediRecord();
        newDedi.Rank = (newIndex + 1);
        newDedi.Login = player.Login;
        newDedi.NickName = player.NickName;
        newDedi.Time = time;
        newDedi.FormattedTime = Time::FormatTime(time);
        newDedi.Checkpoints = player.CurrentCheckpoints;

        if(newIndex == records.size())
        {
            records.push_back(newDedi);
        }
        else
        {
            records.insert((records.begin() + newIndex), newDedi);
        }

        for(int newId = 0; newId < newRecords.size(); newId++)
        {
            DediRecord tempNew = newRecords.at(newId);
            if(tempNew.Login == player.Login)
            {
                newRecords.erase((newRecords.begin() + newId));
            }
        }

        newRecords.push_back(newDedi);

        std::stringstream chatMessage;
        chatMessage << "$fff" << Formatting::StripColors(player.NickName) << "$z$s$0b3 ";
        chatMessage << "drove the $fff" << (newIndex+1) << ".$0b3 Dedimania record with a time of: $fff" << newDedi.FormattedTime << "$0b3";
        if(recordIndex > -1)
        {
            chatMessage << " ($fff" << (recordIndex+1) << ".$0b3, $fff-" << Time::FormatTime((playerRecord.Time - newDedi.Time)) << "$0b3)";
        }
        chatMessage << "!";

        controller->Server->ChatSendServerMessage(chatMessage.str());

        if(!widget.DisplayToAll(controller->Players))
        {
            Logging::PrintError(controller->Server->GetCurrentError());
        }
    }
    else if(time == playerRecord.Time)
    {
        std::stringstream chatMessage;
        chatMessage << "$fff" << Formatting::StripColors(player.NickName) << "$z$s$0b3 equalled the $fff" << (recordIndex+1) << ".$0b3 Dedimania record ($fff" << playerRecord.FormattedTime << "$0b3).";
        controller->Server->ChatSendServerMessage(chatMessage.str());
    }
}

void DedimaniaPlugin::OnEndMatch(std::vector<PlayerRanking> rankings)
{
    if(sessionId == "")
        return;

    if(!mapValid)
        return;

    if(newRecords.size() == 0)
        return;

    std::sort(newRecords.begin(), newRecords.end(), [=](const DediRecord& a, const DediRecord& b) { return a.Time < b.Time; });

    GbxParameters times = GbxParameters();
    std::vector<GbxStructParameters> timesVector = std::vector<GbxStructParameters>();
    for(int newDediId = 0; newDediId < newRecords.size(); newDediId++)
    {
        DediRecord newDedi = newRecords.at(newDediId);

        GbxStructParameters dediStruct = GbxStructParameters();
        dediStruct.Put("Login", "<string>" + newDedi.Login + "</string>");
        dediStruct.Put("Best", "<int>" + std::to_string(newDedi.Time) + "</int>");
        std::string checks = newDedi.GetCheckpoints();
        dediStruct.Put("Checks", "<string>" + checks + "</string>");
        timesVector.push_back(dediStruct);
    }

    for(int timeId = 0; timeId < timesVector.size(); timeId++)
    {
        times.Put(&timesVector.at(timeId));
    }

    std::string gameMode = GameModeConverter::GetDediName(controller->Info->Mode);

    DediRecord topRecord = newRecords.at(0);
    std::string vreplay = controller->Server->GetValidationReplay(topRecord.Login);
    std::string vreplayChecks = topRecord.GetCheckpoints();
    std::string greplay = "";
    if(topRecord.Rank == 1)
    {
        std::string fileName = controller->Maps->Current->UId + "." + gameMode + "." + std::to_string(topRecord.Time) + "." + topRecord.Login + ".Replay.Gbx";
        greplay = controller->Server->GetGhostReplay(topRecord.Login, fileName);
    }

    GbxStructParameters setTimes = GbxStructParameters();
    std::string methodName = "dedimania.SetChallengeTimes";
    setTimes.Put("methodName", Parameter(&methodName));
    GbxParameters parameters = GbxParameters();
    parameters.Put(&sessionId);
        GbxStructParameters mapInfo = GbxStructParameters();
        mapInfo.Put("UId", Parameter(&controller->Maps->Current->UId));
        std::string mapName = Text::EscapeXML(controller->Maps->Current->Name);
        mapInfo.Put("Name", Parameter(&mapName));
        mapInfo.Put("Environment", Parameter(&controller->Maps->Current->Environment));
        mapInfo.Put("Author", Parameter(&controller->Maps->Current->Author));
        mapInfo.Put("NbCheckpoints", Parameter(&controller->Maps->Current->NbCheckpoints));
        mapInfo.Put("NbLaps", Parameter(&controller->Maps->Current->NbLaps));
    parameters.Put(&mapInfo);
    parameters.Put(&gameMode);
    parameters.Put(&times);
    GbxStructParameters replays = GbxStructParameters();
        replays.Put("VReplay", "<base64>" + vreplay + "</base64>");
        replays.Put("VReplayChecks", Parameter(&vreplayChecks));
        replays.Put("Top1GReplay", "<base64>" + greplay + "</base64>");
    parameters.Put(&replays);
    setTimes.Put("params", Parameter(&parameters));
    currentCalls.push_back(setTimes);

    GbxResponse queryResponse = multicall();
    if(!hasError)
    {
        std::cout << "[    \033[0;32mOK\033[0;0m    ] Successfully submitted times to Dedimania." << std::endl << std::flush;
    }
    else
    {
        std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Unable to submit times to Dedimania." << std::endl << std::flush;
    }

    newRecords = std::vector<DediRecord>();
}

void DedimaniaPlugin::OpenDediRecords(Player player)
{
    UIList list = UIList();
    list.Id = "DediRecords";
    list.Title = "$l[http://dedimania.net/tm2stats/?do=stat&Envir=" + controller->Maps->Current->Environment + "&RecOrder3=RANK-ASC&UId=" + controller->Maps->Current->UId + "&Show=RECORDS]Dedimania Records$l for: $z$s$fff" + controller->Maps->Current->Name;
    list.IconStyle = "BgRaceScore2";
    list.IconSubstyle = "LadderRank";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Player", 40));
    list.Columns.push_back(std::pair<std::string, int>("Time", 20));

    for(int recordId = 0; recordId < records.size(); recordId++)
    {
        DediRecord record = records.at(recordId);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", std::to_string(record.Rank)));
        row.insert(std::pair<std::string, std::string>("Player", record.NickName));
        row.insert(std::pair<std::string, std::string>("Time", record.FormattedTime));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

void DedimaniaPlugin::loadSettings()
{
    std::map<std::string, std::string>::iterator limitIt = Settings.find("code");
    if(limitIt != Settings.end())
        dedimaniaCode = limitIt->second;

    std::map<std::string, std::string>::iterator widgetEntriesIt = Settings.find("widgetEntries");
    if(widgetEntriesIt != Settings.end())
        widgetEntries = atoi(widgetEntriesIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetTopCountIt = Settings.find("widgetTopCount");
    if(widgetTopCountIt != Settings.end())
        widgetTopCount = atoi(widgetTopCountIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetXIt = Settings.find("widgetX");
    if(widgetXIt != Settings.end())
        widgetX = atof(widgetXIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetYIt = Settings.find("widgetY");
    if(widgetYIt != Settings.end())
        widgetY = atof(widgetYIt->second.c_str());
}

void DedimaniaPlugin::authenticate()
{
    std::cout << "[          ] Connecting with Dedimania on dedimania.net:8081 ... " << std::endl << std::flush;

    GbxStructParameters session = GbxStructParameters();
    std::string methodName = "dedimania.OpenSession";
    session.Put("methodName", Parameter(&methodName));
    GbxStructParameters params = GbxStructParameters();
    std::string game = "TM2";
    std::string tool = "Mania++";
    params.Put("Game", Parameter(&game));
    params.Put("Login", Parameter(&controller->Info->System.ServerLogin));
    params.Put("Code", Parameter(&dedimaniaCode));
    params.Put("Path", Parameter(&controller->Info->Account.Path));
    params.Put("Packmask", Parameter(&controller->Maps->Current->Environment));
    params.Put("ServerVersion", Parameter(&controller->Info->Version.Version));
    params.Put("ServerBuild", Parameter(&controller->Info->Version.Build));
    params.Put("Tool", Parameter(&tool));
    params.Put("Version", Parameter(&controller->Info->ControllerVersion));
    GbxParameters parameters = GbxParameters();
    parameters.Put(&params);
    session.Put("params", Parameter(&parameters));
    currentCalls.push_back(session);

    GbxResponse queryResponse = multicall();
    if(!hasError)
    {
        std::vector<GbxResponseParameter> responseParams = queryResponse.GetParameters().at(0).GetArray();
        sessionId = responseParams.at(0).GetArray().at(0).GetStruct().at("SessionId").GetString();

        std::cout << "\x1b[1A[    \033[0;32mOK\033[0;0m    ] Connected with Dedimania (session: " << sessionId << ")!" << std::endl << std::flush;
    }
    else
    {
        std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Failed to connect with Dedimania!" << std::endl << std::flush;
    }
}

size_t DedimaniaPlugin::receiveData(void* contents, size_t size, size_t nmemb, std::string* s)
{
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try
    {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }

    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

GbxResponse DedimaniaPlugin::multicall()
{
    GbxParameters params = GbxParameters();
    GbxParameters structArray = GbxParameters();

    GbxStructParameters ttr = GbxStructParameters();
    std::string methodName = "dedimania.WarningsAndTTR";
    ttr.Put("methodName", Parameter(&methodName));
    GbxParameters parameters = GbxParameters();
    ttr.Put("params", Parameter(&parameters));
    currentCalls.push_back(ttr);

    for(int callId = 0; callId < currentCalls.size(); callId++)
    {
        structArray.Put(&(currentCalls.at(callId)));
    }
    params.Put(&structArray);

    GbxResponse response = query(GbxMessage("system.multicall", params));
    currentCalls = std::vector<GbxStructParameters>();
    return response;
}

GbxResponse DedimaniaPlugin::query(GbxMessage message)
{
    hasError = false;
    std::string output;
    CURLcode result;
    std::string tags;

    std::string text = message.GetXml();
    //std::cout << "Text: " << text << std::endl;
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(text.c_str()), text.length());
    //std::cout << "Encoded: " << encoded << std::endl;

    std::stringstream postfields;
    postfields << "xmlrpc=" << encoded;

    //std::string gzipped = GZip::Compress(postfields.str());

    curl = curl_easy_init();

    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, "Cache-Control: no-cache");
    headers = curl_slist_append(headers, "Keep-Alive: timeout=600, max=2000");
    headers = curl_slist_append(headers, "Connection: Keep-Alive");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");

    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_URL, "dedimania.net:8081/Dedimania");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mania++/0.3.0");
    curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate,gzip");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DedimaniaPlugin::receiveData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, strlen(postfields.str().c_str()));
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postfields.str().c_str());

    result = curl_easy_perform(curl);
    if(result != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);

    GbxResponse response;
    response.SetRaw(output);
    //std::cout << "Response: " << output << std::endl;

    std::vector<GbxResponseParameter> responseParams = response.GetParameters().at(0).GetArray();
    if(responseParams.size() != 0)
    {
        std::map<std::string, GbxResponseParameter> warningsAndTTR = responseParams.at((responseParams.size() - 1)).GetArray().at(0).GetStruct();
        std::vector<GbxResponseParameter> warnings = warningsAndTTR.at("methods").GetArray();
        for(int paramId = 0; paramId < warnings.size(); paramId++)
        {
            std::map<std::string, GbxResponseParameter> warning = warnings.at(paramId).GetStruct();
            if(warning.at("errors").GetString().length() > 1)
            {
                hasError = true;
                std::stringstream errorMessage;
                errorMessage << "(" << warning.at("methodName").GetString() << ") " << warning.at("errors").GetString();
                Logging::PrintError(-200, errorMessage.str());
            }
        }
    }

    curl_slist_free_all(headers);

    return response;
}
