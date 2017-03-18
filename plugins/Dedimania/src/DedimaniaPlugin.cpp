#include "DedimaniaPlugin.h"

DedimaniaPlugin::DedimaniaPlugin()
{
    Version = "0.1.0";
    Author = "TheM";
    Description = "Synchonizes records with the global Dedimania server.";

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

    RegisterCommand("dedirecs", "Display list of current Dedimania records.", [this](Player player, std::vector<std::string> parameters) { OpenDediRecords(player); });
    RegisterCommand("dedirecords", "Display list of current Dedimania records.", [this](Player player, std::vector<std::string> parameters) { OpenDediRecords(player); });

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
