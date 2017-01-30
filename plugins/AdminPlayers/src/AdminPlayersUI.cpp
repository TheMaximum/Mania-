#include "AdminPlayersPlugin.h"

void AdminPlayersPlugin::ChatShowBanList(Player player)
{
    std::vector<BannedPlayer> banList = controller->Server->GetBanList(10240, 0);

    UIList list = UIList();
    list.Id = "BanList";
    list.Title = "Players currently banned";
    list.IconStyle = "Icons64x64_1";
    list.IconSubstyle = "IconPlayers";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Login", 30));
    list.Columns.push_back(std::pair<std::string, int>("Client name", 30));
    list.Columns.push_back(std::pair<std::string, int>("IP address", 30));

    for(int itemId = 0; itemId < banList.size(); itemId++)
    {
        BannedPlayer item = banList[itemId];

        std::stringstream index;
        index << (itemId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Login", item.Login));
        row.insert(std::pair<std::string, std::string>("Client name", item.ClientName));
        row.insert(std::pair<std::string, std::string>("IP address", item.IPAddress));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

void AdminPlayersPlugin::ChatShowBlackList(Player player)
{
    std::vector<std::string> blackList = controller->Server->GetBlackList(10240, 0);

    UIList list = UIList();
    list.Id = "BlackList";
    list.Title = "Players currently blacklisted";
    list.IconStyle = "Icons64x64_1";
    list.IconSubstyle = "IconPlayers";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Login", 60));

    for(int itemId = 0; itemId < blackList.size(); itemId++)
    {
        std::string item = blackList[itemId];

        std::stringstream index;
        index << (itemId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Login", item));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

void AdminPlayersPlugin::ChatShowGuestList(Player player)
{
    std::vector<std::string> guestList = controller->Server->GetBlackList(10240, 0);

    UIList list = UIList();
    list.Id = "GuestList";
    list.Title = "Players currently guested";
    list.IconStyle = "Icons64x64_1";
    list.IconSubstyle = "IconPlayers";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Login", 60));

    for(int itemId = 0; itemId < guestList.size(); itemId++)
    {
        std::string item = guestList[itemId];

        std::stringstream index;
        index << (itemId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Login", item));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

void AdminPlayersPlugin::ChatShowIgnoreList(Player player)
{
    std::vector<std::string> ignoreList = controller->Server->GetIgnoreList(10240, 0);

    UIList list = UIList();
    list.Id = "IgnoreList";
    list.Title = "Players currently ignored";
    list.IconStyle = "Icons64x64_1";
    list.IconSubstyle = "IconPlayers";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Login", 60));

    for(int itemId = 0; itemId < ignoreList.size(); itemId++)
    {
        std::string item = ignoreList[itemId];

        std::stringstream index;
        index << (itemId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Login", item));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}
