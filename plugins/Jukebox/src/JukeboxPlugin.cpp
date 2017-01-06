#include "JukeboxPlugin.h"

JukeboxPlugin::JukeboxPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    RegisterCommand("maps", [this](Player player, std::vector<std::string> parameters) { DisplayMapList(player); });
}

void JukeboxPlugin::Init()
{
    loadSettings();
}

void JukeboxPlugin::loadSettings()
{
    /*std::map<std::string, std::string>::iterator widgetXIt = Settings.find("widgetX");
    if(widgetXIt != Settings.end())
        widgetX = atof(widgetXIt->second.c_str());*/
}

void JukeboxPlugin::DisplayMapList(Player player)
{
    std::vector<std::pair<std::string, Map>> mapsVector;
    std::copy(controller->Maps->List.begin(), controller->Maps->List.end(), back_inserter(mapsVector));
    std::sort(mapsVector.begin(), mapsVector.end(), [=](const std::pair<std::string, Map>& a, const std::pair<std::string, Map>& b) { return a.second.Id > b.second.Id; });

    UIList list = UIList();
    list.Id = "MapList";
    list.Title = "Server map list";
    list.IconStyle = "Icons128x128_1";
    list.IconSubstyle = "CustomStars";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Name", 40));
    list.Columns.push_back(std::pair<std::string, int>("Author", 20));

    for(int mapId = 0; mapId < mapsVector.size(); mapId++)
    {
        Map mapInList = mapsVector[mapId].second;

        std::stringstream index;
        index << (mapId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Name", mapInList.Name));
        row.insert(std::pair<std::string, std::string>("Author", mapInList.Author));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}
