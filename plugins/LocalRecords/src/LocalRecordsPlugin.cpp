#include "LocalRecordsPlugin.h"

LocalRecordsPlugin::LocalRecordsPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    PlayerFinish.push_back([this](Player player, int time) { OnPlayerFinish(player, time); });
    PlayerCheckpoint.push_back([this](Player player, int time, int currentLap, int checkpointIndex) { OnPlayerCheckpoint(player, time, currentLap, checkpointIndex); });
}

void LocalRecordsPlugin::Init()
{
    retrieveRecords(*controller->Maps->Current);
    std::cout << "[  INFO   ] " << localRecords.size() << " records found for " << controller->Maps->Current->Name << "." << std::endl;

    widget = LocalRecordsWidget(controller->UI, &localRecords);
    controller->UI->AddEvent("OpenLocalRecords", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { OpenLocalRecords(player, answer, entries); }));

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

void LocalRecordsPlugin::OnBeginMap(Map map)
{
    retrieveRecords(*controller->Maps->Current);
    std::cout << "[  INFO   ] " << localRecords.size() << " records found for " << controller->Maps->Current->Name << "." << std::endl;

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

void LocalRecordsPlugin::OnPlayerFinish(Player player, int time)
{
    std::cout << "FINISH! Time " << time << ", checkpoints: ";
    for(int checkPointId = 0; checkPointId < player.CurrentCheckpoints.size(); checkPointId++)
    {
        if(checkPointId > 0) std::cout << ",";
        std::cout << player.CurrentCheckpoints.at(checkPointId);
    }
    std::cout << std::endl;
}

void LocalRecordsPlugin::OnPlayerCheckpoint(Player player, int time, int currentLap, int checkpointIndex)
{
    std::cout << "CHECKPOINT! List: ";
    for(int checkPointId = 0; checkPointId < player.CurrentCheckpoints.size(); checkPointId++)
    {
        if(checkPointId > 0) std::cout << ",";
        std::cout << player.CurrentCheckpoints.at(checkPointId);
    }
    std::cout << std::endl;
}

void LocalRecordsPlugin::OpenLocalRecords(Player player, std::string answer, std::vector<EntryVal> entries)
{
    std::cout << "Player '" << player.Login << "' has clicked the LocalRecords widget (" << answer << ")!" << std::endl;
}

void LocalRecordsPlugin::displayPersonalRecord(Player player)
{
    bool playerFound = false;
    for(int recordId = 0; recordId < localRecords.size(); recordId++)
    {
        LocalRecord localRecord = localRecords.at(recordId);
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
    localRecords = std::vector<LocalRecord>();

    sql::PreparedStatement* pstmt = controller->Database->prepareStatement("SELECT * FROM `records` WHERE `MapId` = ? ORDER BY `Score` ASC LIMIT ?");
    pstmt->setInt(1, map.Id);
    pstmt->setInt(2, 100);
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

        localRecords.push_back(localRecord);

        delete pstmt; pstmt = NULL;
        delete playerResult; playerResult = NULL;
    }

    delete pstmt; pstmt = NULL;
    delete result; result = NULL;
}
