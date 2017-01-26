#include "LocalRecordsPlugin.h"

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
