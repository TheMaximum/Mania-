#include "LocalRecordsPlugin.h"

LocalRecordsPlugin::LocalRecordsPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
}

void LocalRecordsPlugin::Init()
{
    retrieveRecords(*controller->Maps->Current);
    std::cout << "[  INFO   ] " << localRecords.size() << " records found for " << controller->Maps->Current->Name << "." << std::endl;

    widget = LocalRecordsWidget(controller->UI, &localRecords);
    controller->UI->AddEvent("OpenLocalRecords", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { OpenLocalRecords(player, answer, entries); }));

    if(!widget.DisplayToAll(controller->Players))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void LocalRecordsPlugin::OnBeginMap(Map map)
{
    retrieveRecords(map);
    std::cout << "[  INFO   ] " << localRecords.size() << " records found for " << map.Name << "." << std::endl;

    if(!widget.DisplayToAll(controller->Players))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void LocalRecordsPlugin::OpenLocalRecords(Player player, std::string answer, std::vector<EntryVal> entries)
{
    std::cout << "Player '" << player.Login << "' has clicked the LocalRecords widget (" << answer << ")!" << std::endl;
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
