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
    std::cout << "PLUGIN Begin map: " << map.Name << "!" << std::endl;

    sql::PreparedStatement* pstmt;
    pstmt = database->prepareStatement("SELECT * FROM `maps` WHERE `Uid` = ?");
    pstmt->setString(1, map.UId);
    sql::ResultSet* result = pstmt->executeQuery();
    result->next();
    std::cout << "ID: " << result->getInt("Id") << std::endl;

    delete pstmt; pstmt = NULL;
}
