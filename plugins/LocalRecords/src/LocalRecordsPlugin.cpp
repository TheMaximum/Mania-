#include "LocalRecordsPlugin.h"

LocalRecordsPlugin::LocalRecordsPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
    PlayerManialinkPageAnswer.push_back([this](Player player, std::string answer, std::vector<EntryVal> entries) { OnPlayerManialinkPageAnswer(player, answer, entries); });
}

void LocalRecordsPlugin::Init()
{
    retrieveRecords(*controller->Maps->Current);
    std::cout << "[  INFO   ] " << localRecords.size() << " records found for " << controller->Maps->Current->Name << "." << std::endl;

    widget = LocalRecordsWidget(controller->Server, &localRecords);
    widget.DisplayToAll(controller->Players);
}

void LocalRecordsPlugin::OnBeginMap(Map map)
{
    retrieveRecords(map);
    std::cout << "[  INFO   ] " << localRecords.size() << " records found for " << map.Name << "." << std::endl;

    widget.DisplayToAll(controller->Players);
}

void LocalRecordsPlugin::OnPlayerManialinkPageAnswer(Player player, std::string answer, std::vector<EntryVal> entries)
{
    if(answer.find(widget.ActionId) != std::string::npos)
    {
        std::cout << "Player '" << player.Login << "' has clicked the LocalRecords widget (" << answer << ")!" << std::endl;
    }
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
