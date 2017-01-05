#include "LocalRecordsPlugin.h"

LocalRecordsPlugin::LocalRecordsPlugin()
{
    Version = "0.2.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    PlayerFinish.push_back([this](Player player, int playerTime) { OnPlayerFinish(player, playerTime); });

    RegisterCommand("records", [this](Player player, std::vector<std::string> parameters) { OpenLocalRecordsCommand(player, parameters); });
}

void LocalRecordsPlugin::Init()
{
    retrieveRecords(*controller->Maps->Current);
    std::cout << "[  INFO   ] " << localRecords.List.size() << " records found for " << controller->Maps->Current->Name << "." << std::endl;

    widget = LocalRecordsWidget(controller->UI, &localRecords);
    controller->UI->AddEvent("OpenLocalRecords", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { OpenLocalRecordsAnswer(player, answer, entries); }));

    if(!widget.DisplayToAll(controller->Players))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void LocalRecordsPlugin::OnBeginMap(Map map)
{
    retrieveRecords(*controller->Maps->Current);
    std::cout << "[  INFO   ] " << localRecords.List.size() << " records found for " << controller->Maps->Current->Name << "." << std::endl;

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

        sql::PreparedStatement* pstmt = controller->Database->prepareStatement("INSERT INTO `rs_times` (`MapId`, `PlayerId`, `Score`, `Date`, `Checkpoints`) VALUES (?, ?, ?, ?, ?)");
        pstmt->setInt(1, controller->Maps->Current->Id);
        pstmt->setInt(2, player.Id);
        pstmt->setInt(3, playerTime);
        pstmt->setInt(4, time(NULL));
        pstmt->setString(5, checkpoints.str());
        pstmt->execute();

        delete pstmt; pstmt = NULL;

        bool recordInserted = false;

        if(currentLocal.Id == 0)
        {
            // Player has no local.
            bool insertNewRecord = false;

            if(localRecords.List.size() == recordLimit)
            {
                // Records list is full.
                LocalRecord lastRecord = localRecords.List.back();
                if(playerTime < lastRecord.Time)
                {
                    // Time is better than last record, so insert in database.
                    insertNewRecord = true;
                }
            }
            else
            {
                // Records list isn't full, so can always be added.
                insertNewRecord = true;
            }

            if(insertNewRecord)
            {
                sql::PreparedStatement* pstmt = controller->Database->prepareStatement("INSERT INTO `records` (`MapId`, `PlayerId`, `Score`, `Date`, `Checkpoints`) VALUES (?, ?, ?, ?, ?)");
                pstmt->setInt(1, controller->Maps->Current->Id);
                pstmt->setInt(2, player.Id);
                pstmt->setInt(3, playerTime);
                pstmt->setString(4, Time::Current());
                pstmt->setString(5, checkpoints.str());
                pstmt->execute();

                recordInserted = true;
                delete pstmt; pstmt = NULL;
            }
        }
        else
        {
            // Player has a local.
            if(playerTime < currentLocal.Time)
            {
                // Player improved his record.
                sql::PreparedStatement* pstmt = controller->Database->prepareStatement("UPDATE `records` SET `Score` = ?, `Date` = ?, `Checkpoints` = ? WHERE `Id` = ?");
                pstmt->setInt(1, playerTime);
                pstmt->setString(2, Time::Current());
                pstmt->setString(3, checkpoints.str());
                pstmt->setInt(4, currentLocal.Id);
                pstmt->execute();

                recordInserted = true;
                delete pstmt; pstmt = NULL;
            }
            else if(playerTime == currentLocal.Time)
            {
                // Player equalled his record.
                std::stringstream chatMessage;
                chatMessage << "$fff" << player.NickName << "$z$s$0f3 equalled the $fff" << currentLocalIndex << ".$0f3 record ($fff" << currentLocal.FormattedTime << "$0f3).";
                controller->Server->ChatSendServerMessage(chatMessage.str());
            }
        }

        retrieveRecords(*controller->Maps->Current);

        if(recordInserted)
        {
            LocalRecord newLocal = localRecords.GetPlayerRecord(player);
            int newLocalIndex = localRecords.GetPlayerRecordIndex(player);

            std::stringstream chatMessage;
            chatMessage << "$fff" << player.NickName << "$z$s$0f3 ";
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
        }

        if(!widget.DisplayToAll(controller->Players))
        {
            Logging::PrintError(controller->Server->GetCurrentError());
        }
    }
}

void LocalRecordsPlugin::OpenLocalRecordsAnswer(Player player, std::string answer, std::vector<EntryVal> entries)
{
    std::cout << "Player '" << player.Login << "' has clicked the LocalRecords widget (" << answer << ")!" << std::endl;
}

void LocalRecordsPlugin::OpenLocalRecordsCommand(Player player, std::vector<std::string> parameters)
{
    std::cout << "Player '" << player.Login << "' has called /records." << std::endl;
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

    sql::PreparedStatement* pstmt = controller->Database->prepareStatement("SELECT * FROM `records` WHERE `MapId` = ? ORDER BY `Score` ASC LIMIT ?");
    pstmt->setInt(1, map.Id);
    pstmt->setInt(2, recordLimit);
    sql::ResultSet* result = pstmt->executeQuery();

    while(result->next())
    {
        delete pstmt; pstmt = NULL;
        LocalRecord localRecord = LocalRecord(result);

        pstmt = controller->Database->prepareStatement("SELECT * FROM `players` WHERE `Id` = ?");
        pstmt->setInt(1, result->getInt("PlayerId"));
        sql::ResultSet* playerResult = pstmt->executeQuery();
        playerResult->next();

        localRecord.Login = playerResult->getString("Login");
        localRecord.NickName = playerResult->getString("NickName");

        localRecords.List.push_back(localRecord);

        delete playerResult; playerResult = NULL;
    }

    if(pstmt != NULL)
        delete pstmt; pstmt = NULL;
    delete result; result = NULL;
}
