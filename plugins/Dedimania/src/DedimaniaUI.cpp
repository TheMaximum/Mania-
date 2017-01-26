#include "DedimaniaPlugin.h"

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
