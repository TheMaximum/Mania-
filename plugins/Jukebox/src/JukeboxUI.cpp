#include "JukeboxPlugin.h"

void JukeboxPlugin::DisplayJukeboxList(Player player)
{
    UIList list = UIList();
    list.Id = "JukeboxList";
    list.Title = "Maps currently in the Jukebox";
    list.IconStyle = "Icons64x64_1";
    list.IconSubstyle = "ToolTree";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Map", 30));
    list.Columns.push_back(std::pair<std::string, int>("Requested by", 30));

    for(int itemId = 0; itemId < jukebox.size(); itemId++)
    {
        JukeboxItem item = jukebox[itemId];

        std::stringstream index;
        index << (itemId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Map", item.map.Name));
        row.insert(std::pair<std::string, std::string>("Requested by", item.player.NickName));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

void JukeboxPlugin::DisplayMapList(Player player, std::vector<std::string> parameters)
{
    std::vector<std::pair<std::string, Map>> mapsVector;
    std::copy(controller->Maps->List.begin(), controller->Maps->List.end(), back_inserter(mapsVector));

    std::string sort = "default";
    if(parameters.size() > 0)
    {
        if(parameters[0].compare("karma") == 0)
        {
            sort = "karma";
            if(parameters.size() > 1)
            {
                if(parameters[1].compare("-") == 0)
                {
                    sort = "-karma";
                }
            }
        }
        else if(parameters[0].compare("shortest") == 0 ||
                parameters[0].compare("longest") == 0)
        {
            sort = parameters[0];
        }
    }

    if(sort.find("default") != std::string::npos)
    {
        std::sort(mapsVector.begin(), mapsVector.end(), [=](const std::pair<std::string, Map>& a, const std::pair<std::string, Map>& b) { return a.second.Id > b.second.Id; });
    }
    else if(sort.find("shortest") != std::string::npos ||
            sort.find("longest") != std::string::npos)
    {
        if(controller->Plugins->IsLoaded("LocalRecords", "0.2.0"))
        {
            std::sort(mapsVector.begin(), mapsVector.end(), [=](const std::pair<std::string, Map>& a, const std::pair<std::string, Map>& b)
            {
                int localA = 0;
                int localB = 0;

                if(a.second.Additionals.find("Local") != a.second.Additionals.end())
                    localA = boost::any_cast<int>(a.second.Additionals.at("Local"));

                if(b.second.Additionals.find("Local") != b.second.Additionals.end())
                    localB = boost::any_cast<int>(b.second.Additionals.at("Local"));

                if(sort.compare("shortest") == 0)
                {
                    return localA < localB;
                }

                return localA > localB;
            });
        }
    }
    else if(sort.find("karma") != std::string::npos)
    {
        if(controller->Plugins->IsLoaded("Karma", "0.2.0"))
        {
            std::sort(mapsVector.begin(), mapsVector.end(), [=](const std::pair<std::string, Map>& a, const std::pair<std::string, Map>& b)
            {
                int karmaA = 0;
                int karmaB = 0;

                if(a.second.Additionals.find("Karma") != a.second.Additionals.end())
                    karmaA = boost::any_cast<int>(a.second.Additionals.at("Karma"));

                if(b.second.Additionals.find("Karma") != b.second.Additionals.end())
                    karmaB = boost::any_cast<int>(b.second.Additionals.at("Karma"));

                if(sort.compare("-karma") == 0)
                {
                    return karmaA < karmaB;
                }

                return karmaA > karmaB;
            });
        }
    }

    UIList list = UIList();
    list.Id = "MapList";
    list.Title = "Server map list";
    list.IconStyle = "Icons64x64_1";
    list.IconSubstyle = "Browser";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Name", 40));
    list.Columns.push_back(std::pair<std::string, int>("Author", 20));
    if(controller->Plugins->IsLoaded("Karma", "0.2.0"))
    {
        list.Columns.push_back(std::pair<std::string, int>("Karma", 10));
    }
    if(controller->Plugins->IsLoaded("LocalRecords", "0.2.0"))
    {
        list.Columns.push_back(std::pair<std::string, int>("Local", 10));
    }

    list.Actions.insert(std::pair<std::string, std::pair<std::string, std::string>>("Name", std::pair<std::string, std::string>("JukeboxMap", "UId")));
    list.Actions.insert(std::pair<std::string, std::pair<std::string, std::string>>("Author", std::pair<std::string, std::string>("ListAuthor", "Author")));

    for(int mapId = 0; mapId < mapsVector.size(); mapId++)
    {
        Map mapInList = mapsVector[mapId].second;

        std::stringstream index;
        index << (mapId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("UId", mapInList.UId));
        row.insert(std::pair<std::string, std::string>("Name", mapInList.Name));
        row.insert(std::pair<std::string, std::string>("Author", mapInList.Author));

        if(controller->Plugins->IsLoaded("Karma", "0.2.0"))
        {
            int karma = 0;
            if(mapInList.Additionals.find("Karma") != mapInList.Additionals.end())
            {
                boost::any mapKarma = mapInList.Additionals.at("Karma");
                if(mapKarma.type() == typeid(int))
                {
                    karma = boost::any_cast<int>(mapKarma);
                }
            }
            else
            {
                boost::any mapKarma = controller->Plugins->CallMethod("Karma", "GetKarmaByMapId", mapInList.Id);
                if(mapKarma.type() == typeid(int))
                {
                    karma = boost::any_cast<int>(mapKarma);
                }
            }

            row.insert(std::pair<std::string, std::string>("Karma", std::to_string(karma)));
        }

        if(controller->Plugins->IsLoaded("LocalRecords", "0.2.0"))
        {
            int local = 0;
            if(mapInList.Additionals.find("Local") != mapInList.Additionals.end())
            {
                boost::any mapRecord = mapInList.Additionals.at("Local");
                if(mapRecord.type() == typeid(int))
                {
                    local = boost::any_cast<int>(mapRecord);
                }
            }
            else
            {
                boost::any mapRecord = controller->Plugins->CallMethod("LocalRecords", "GetLocalByMapId", mapInList.Id);
                if(mapRecord.type() == typeid(int))
                {
                    local = boost::any_cast<int>(mapRecord);
                }
            }

            row.insert(std::pair<std::string, std::string>("Local", Time::FormatTime(local)));
        }

        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}
