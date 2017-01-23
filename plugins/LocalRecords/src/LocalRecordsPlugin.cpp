#include "LocalRecordsPlugin.h"

LocalRecordsPlugin::LocalRecordsPlugin()
{
    Version = "0.2.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    PlayerFinish.push_back([this](Player player, int playerTime) { OnPlayerFinish(player, playerTime); });

    RegisterCommand("records", [this](Player player, std::vector<std::string> parameters) { OpenLocalRecords(player); });
    RegisterCallableMethod("GetLocalByMapId", [this](boost::any parameters) { return GetLocalByMapId(parameters); });
}

void LocalRecordsPlugin::Init()
{
    loadSettings();
    updateAllMapLocals();

    retrieveRecords(*controller->Maps->Current);
    std::cout << "[    \033[0;32mOK\033[0;0m    ] " << localRecords.List.size() << " records found for '" << Formatting::StripColors(controller->Maps->Current->Name, false) << "'." << std::endl;

    widget = LocalRecordsWidget(controller->UI, &localRecords);
    widget.WidgetEntries = widgetEntries;
    widget.WidgetTopCount = widgetTopCount;
    widget.WidgetX = widgetX;
    widget.WidgetY = widgetY;
    controller->UI->RegisterEvent("OpenLocalRecords", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { OpenLocalRecords(player); }));

    if(!widget.DisplayToAll(controller->Players))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void LocalRecordsPlugin::loadSettings()
{
    std::map<std::string, std::string>::iterator limitIt = Settings.find("limit");
    if(limitIt != Settings.end())
        recordLimit = atoi(limitIt->second.c_str());

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

void LocalRecordsPlugin::OnBeginMap(Map map)
{
    retrieveRecords(*controller->Maps->Current);
    std::cout << "[    \033[0;32mOK\033[0;0m    ] " << localRecords.List.size() << " records found for '" << Formatting::StripColors(controller->Maps->Current->Name, false) << "'." << std::endl;

    for(std::map<std::string, Player>::iterator player = controller->Players->begin(); player != controller->Players->end(); ++player)
    {
        Player currentPlayer = player->second;
        displayPersonalRecord(currentPlayer);
        if(!widget.DisplayToPlayer(currentPlayer))
        {
            Logging::PrintError(controller->Server->GetCurrentError());
        }
    }
}

void LocalRecordsPlugin::OnPlayerConnect(Player player)
{
    displayPersonalRecord(player);
    if(!widget.DisplayToPlayer(player))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void LocalRecordsPlugin::OnPlayerFinish(Player player, int playerTime)
{
    if(playerTime > 0 && player.CurrentCheckpoints.size() == controller->Maps->Current->NbCheckpoints)
    {
        LocalRecord currentLocal = localRecords.GetPlayerRecord(player);
        int currentLocalIndex = localRecords.GetPlayerRecordIndex(player);

        std::stringstream checkpoints;
        for(int checkPointId = 0; checkPointId < player.CurrentCheckpoints.size(); checkPointId++)
        {
            if(checkPointId > 0) checkpoints << ",";
            checkpoints << player.CurrentCheckpoints.at(checkPointId);
        }

        sql::PreparedStatement* pstmtTimes;

        try
        {
            pstmtTimes = controller->Database->prepareStatement("INSERT INTO `times` (`MapId`, `PlayerId`, `Score`, `Date`, `Checkpoints`) VALUES (?, ?, ?, ?, ?)");
            pstmtTimes->setInt(1, controller->Maps->Current->Id);
            pstmtTimes->setInt(2, player.Id);
            pstmtTimes->setInt(3, playerTime);
            pstmtTimes->setInt(4, time(NULL));
            pstmtTimes->setString(5, checkpoints.str());
            pstmtTimes->execute();
        }
        catch(sql::SQLException &e)
        {
            std::cout << "Failed to save time for " << player.Login << " on '" << controller->Maps->Current->Name << "' ..." << std::endl;
            Logging::PrintError(e.getErrorCode(), e.what());
        }

        if(pstmtTimes != NULL)
        {
            delete pstmtTimes;
            pstmtTimes = NULL;
        }

        bool updateRecord = false;
        bool recordInserted = false;

        if(currentLocal.Id == 0)
        {
            // Player has no local.
            if(localRecords.List.size() == recordLimit)
            {
                // Records list is full.
                LocalRecord lastRecord = localRecords.List.back();
                if(playerTime < lastRecord.Time)
                {
                    // Time is better than last record, so insert in database.
                    updateRecord = true;
                }
            }
            else
            {
                // Records list isn't full, so can always be added.
                updateRecord = true;
            }
        }
        else
        {
            // Player has a local.
            if(playerTime < currentLocal.Time)
            {
                // Player improved his record.
                updateRecord = true;
            }
            else if(playerTime == currentLocal.Time)
            {
                // Player equalled his record.
                std::stringstream chatMessage;
                chatMessage << "$fff" << Formatting::StripColors(player.NickName) << "$z$s$0f3 equalled the $fff" << currentLocalIndex << ".$0f3 record ($fff" << currentLocal.FormattedTime << "$0f3).";
                controller->Server->ChatSendServerMessage(chatMessage.str());
            }
        }

        if(updateRecord)
        {
            sql::PreparedStatement* pstmt;
            try
            {
                pstmt = controller->Database->prepareStatement("INSERT INTO `records` (`MapId`, `PlayerId`, `Score`, `Date`, `Checkpoints`) VALUES (?, ?, ?, ?, ?) ON DUPLICATE KEY UPDATE `Score` = VALUES(`Score`), `Date` = VALUES(`Date`), `Checkpoints` = VALUES(`Checkpoints`)");
                pstmt->setInt(1, controller->Maps->Current->Id);
                pstmt->setInt(2, player.Id);
                pstmt->setInt(3, playerTime);
                pstmt->setString(4, Time::Current());
                pstmt->setString(5, checkpoints.str());
                pstmt->execute();

                recordInserted = true;
            }
            catch(sql::SQLException &e)
            {
                std::cout << "Failed to save record for " << player.Login << " on '" << controller->Maps->Current->Name << "' ..." << std::endl;
                Logging::PrintError(e.getErrorCode(), e.what());
            }

            if(pstmt != NULL)
            {
                delete pstmt;
                pstmt = NULL;
            }
        }

        if(recordInserted)
        {
            retrieveRecords(*controller->Maps->Current);

            LocalRecord newLocal = localRecords.GetPlayerRecord(player);
            int newLocalIndex = localRecords.GetPlayerRecordIndex(player);

            std::stringstream chatMessage;
            chatMessage << "$fff" << Formatting::StripColors(player.NickName) << "$z$s$0f3 ";
            chatMessage << "drove the $fff" << newLocalIndex << ".$0f3 record with a time of: $fff" << newLocal.FormattedTime << "$0f3";
            if(currentLocal.Id == 0)
            {
                chatMessage << "!";
            }
            else
            {
                chatMessage << " ($fff" << currentLocalIndex << ".$0f3, $fff-" << Time::FormatTime((currentLocal.Time - newLocal.Time)) << "$0f3)!";
            }

            controller->Server->ChatSendServerMessage(chatMessage.str());

            if(newLocalIndex == 1)
            {
                controller->Maps->Current->UpdateAdditional("Local", newLocal.Time);
            }

            if(!widget.DisplayToAll(controller->Players))
            {
                Logging::PrintError(controller->Server->GetCurrentError());
            }
        }
    }
}

void LocalRecordsPlugin::OpenLocalRecords(Player player)
{
    UIList list = UIList();
    list.Id = "LocalRecords";
    list.Title = "Local Records for: $z$s$fff" + controller->Maps->Current->Name;
    list.IconStyle = "BgRaceScore2";
    list.IconSubstyle = "LadderRank";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Player", 40));
    list.Columns.push_back(std::pair<std::string, int>("Time", 20));

    for(int recordId = 0; recordId < localRecords.List.size(); recordId++)
    {
        LocalRecord record = localRecords.List.at(recordId);

        std::stringstream index;
        index << (recordId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Player", record.NickName));
        row.insert(std::pair<std::string, std::string>("Time", record.FormattedTime));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

boost::any LocalRecordsPlugin::GetLocalByMapId(boost::any parameters)
{
    boost::any result = 0;
    int mapId = boost::any_cast<int>(parameters);
    if(mapId != 0)
    {
        sql::PreparedStatement* pstmt;
        sql::ResultSet* dbResult;
        try
        {
            pstmt = controller->Database->prepareStatement("SELECT Score FROM `records` WHERE `MapId` = ? ORDER BY Score ASC LIMIT 1");
            pstmt->setInt(1, mapId);
            dbResult = pstmt->executeQuery();
            dbResult->next();
            result = dbResult->getInt("Score");
        }
        catch(sql::SQLException &e)
        {
            // No record.
        }

        if(pstmt != NULL)
        {
            delete pstmt;
            pstmt = NULL;
        }

        if(dbResult != NULL)
        {
            delete dbResult;
            dbResult = NULL;
        }
    }

    return result;
}

void LocalRecordsPlugin::updateAllMapLocals()
{
    std::cout << "[          ] Retrieving locals for all maps ... " << '\r' << std::flush;
    for(std::map<std::string, Map>::iterator mapIt = controller->Maps->List.begin(); mapIt != controller->Maps->List.end(); ++mapIt)
    {
        mapIt->second.UpdateAdditional("Local", GetLocalByMapId(mapIt->second.Id));
    }
    Logging::PrintOKFlush();
}

void LocalRecordsPlugin::displayPersonalRecord(Player player)
{
    bool playerFound = false;
    for(int recordId = 0; recordId < localRecords.List.size(); recordId++)
    {
        LocalRecord localRecord = localRecords.List.at(recordId);
        if(localRecord.Login == player.Login)
        {
            std::stringstream chatMessage;
            chatMessage << "$0f3Your current Local Record:$fff " << localRecord.FormattedTime << " $0f3($fff" << (recordId + 1) << ".$0f3)";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
            playerFound = true;
        }
    }

    if(!playerFound)
        controller->Server->ChatSendServerMessageToLogin("$0f3You currently don't have a Local Record on this map.", player.Login);
}

void LocalRecordsPlugin::retrieveRecords(Map map)
{
    localRecords.List = std::vector<LocalRecord>();

    sql::PreparedStatement* pstmt;
    sql::ResultSet* result;

    try
    {
        pstmt = controller->Database->prepareStatement("SELECT * FROM `records` WHERE `MapId` = ? ORDER BY `Score` ASC LIMIT ?");
        pstmt->setInt(1, map.Id);
        pstmt->setInt(2, recordLimit);
        result = pstmt->executeQuery();

        while(result->next())
        {
            LocalRecord localRecord = LocalRecord(result);

            sql::PreparedStatement* playerPstmt = controller->Database->prepareStatement("SELECT * FROM `players` WHERE `Id` = ?");
            playerPstmt->setInt(1, result->getInt("PlayerId"));
            sql::ResultSet* playerResult = playerPstmt->executeQuery();
            playerResult->next();

            try
            {
                localRecord.Login = playerResult->getString("Login");
                localRecord.NickName = playerResult->getString("NickName");
            }
            catch(sql::InvalidArgumentException &e)
            {
                // Player cannot be found in the database.
                localRecord.Login = "";
                localRecord.NickName = "";
            }

            localRecords.List.push_back(localRecord);

            if(playerResult != NULL)
            {
                delete playerResult;
                playerResult = NULL;
            }

            delete playerPstmt; playerPstmt = NULL;
        }

        delete pstmt; pstmt = NULL;
    }
    catch(sql::SQLException &e)
    {
        std::cout << "Failed to retrieve records for '" << map.Name << "' ..." << std::endl;
        Logging::PrintError(e.getErrorCode(), e.what());
    }

    if(pstmt != NULL)
    {
        delete pstmt;
        pstmt = NULL;
    }

    if(result != NULL)
    {
        delete result; result = NULL;
    }
}
