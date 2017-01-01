#include "LocalRecordsPlugin.h"

LocalRecordsPlugin::LocalRecordsPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
}

void LocalRecordsPlugin::Init()
{

}

void LocalRecordsPlugin::OnBeginMap(Map map)
{
    sql::PreparedStatement* pstmt = database->prepareStatement("SELECT * FROM `records` WHERE `MapId` = ? ORDER BY `Score` ASC LIMIT ?");
    pstmt->setInt(1, map.Id);
    pstmt->setInt(2, 100);
    sql::ResultSet* result = pstmt->executeQuery();

    std::cout << "Records for " << map.Name << ":" << std::endl;
    int recordIndex = 1;
    while(result->next())
    {
        std::cout << "#" << recordIndex << ": " << Time::FormatTime(result->getInt("Score")) << " by " << result->getInt("PlayerId") << std::endl;
        recordIndex++;
    }

    delete pstmt; pstmt = NULL;
    delete result; result = NULL;
}
