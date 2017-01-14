#include "JukeboxPlugin.h"

JukeboxPlugin::JukeboxPlugin()
{
    Version = "0.2.0";
    Author = "TheM";

    EndMatch.push_back([this](std::vector<PlayerRanking> rankings, int winnerTeam) { OnEndMatch(); });
    RegisterCommand("list", [this](Player player, std::vector<std::string> parameters) { DisplayMapList(player); });
    RegisterCommand("jukebox", [this](Player player, std::vector<std::string> parameters) { ChatJukebox(player, parameters); });
}

void JukeboxPlugin::Init()
{
    loadSettings();
    controller->UI->RegisterEvent("JukeboxMap", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { JukeboxMap(player, answer); }));
}

void JukeboxPlugin::loadSettings()
{
    std::map<std::string, std::string>::iterator skipMapWhenLeftIt = Settings.find("skipMapWhenLeft");
    if(skipMapWhenLeftIt != Settings.end())
        std::istringstream(skipMapWhenLeftIt->second) >> skipMapWhenLeft;
}

void JukeboxPlugin::OnEndMatch()
{
    if(skipMapWhenLeft)
    {
        for (std::vector<JukeboxItem>::iterator jukeIt = (jukebox.end() - 1); jukeIt != (jukebox.begin() - 1); --jukeIt)
        {
            std::map<std::string, Player>::iterator findPlayer = controller->Players->find(jukeIt->player.Login);
            if(findPlayer == controller->Players->end())
            {
                std::stringstream skipMessage;
                skipMessage << "$fa0Map $fff" << jukeIt->map.Name << "$z$s$fa0 skipped because requester $fff" << jukeIt->player.NickName << "$z$s$fa0 left.";
                controller->Server->ChatSendServerMessage(skipMessage.str());
                jukeIt = jukebox.erase(jukeIt);
            }
        }
    }

    if(jukebox.size() > 0)
    {
        JukeboxItem nextMap = jukebox.at(0);

        std::stringstream nextMessage;
        nextMessage << "$fa0The next map will be $fff" << nextMap.map.Name << "$z$s$fa0 as requested by $fff" << nextMap.player.NickName << "$z$s$fa0.";
        controller->Server->ChatSendServerMessage(nextMessage.str());

        controller->Server->ChooseNextMap(nextMap.map.FileName);
        jukebox.erase(jukebox.begin());
    }
}

void JukeboxPlugin::ChatJukebox(Player player, std::vector<std::string> parameters)
{
    if(parameters.size() > 0)
    {
        if(parameters.at(0).find("display") != std::string::npos ||
           parameters.at(0).find("list") != std::string::npos)
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
        else if(parameters.at(0).find("drop") != std::string::npos)
        {
            for(int jukeId = 0; jukeId < jukebox.size(); jukeId++)
            {
                if(jukebox.at(jukeId).player.Login == player.Login)
                {
                    std::stringstream dropMessage;
                    dropMessage << "$fff" << player.NickName << "$z$s$fa0 dropped $fff" << jukebox.at(jukeId).map.Name << "$z$s$fa0 from the jukebox.";
                    controller->Server->ChatSendServerMessage(dropMessage.str());

                    jukebox.erase((jukebox.begin() + jukeId));
                    return;
                }
            }
        }
    }
    else
    {
        std::stringstream helpMessage;
        helpMessage << "$fff/jukebox$fa0 options: $fffdisplay/list$fa0 (displays current Jukebox list), $fffdrop$fa0 (drops your Jukebox entry).";
        controller->Server->ChatSendServerMessageToLogin(helpMessage.str(), player.Login);
    }
}

void JukeboxPlugin::JukeboxMap(Player player, std::string answer)
{
    int paramBegin = (answer.find('(') + 1);
    int paramEnd = answer.find(')');
    std::string mapUid = answer.substr(paramBegin, (paramEnd - paramBegin));

    for(int jukeId = 0; jukeId < jukebox.size(); jukeId++)
    {
        if(jukebox.at(jukeId).map.UId == mapUid)
        {
            controller->Server->ChatSendServerMessageToLogin("$fa0This map has already been added to the jukebox, pick another one.", player.Login);
            return;
        }

        if(jukebox.at(jukeId).player.Login == player.Login)
        {
            controller->Server->ChatSendServerMessageToLogin("$fa0You already have a map in the jukebox! Wait till it's been played before adding another.", player.Login);
            return;
        }
    }

    std::map<std::string, Map>::iterator mapIt = controller->Maps->List.find(mapUid);
    if(mapIt != controller->Maps->List.end())
    {
        Map map = mapIt->second;
        jukebox.push_back({ map, player });

        std::stringstream nextMessage;
        nextMessage << "$fff" << map.Name << "$z$s$fa0 was added to the jukebox by $fff" << player.NickName << "$z$s$fa0.";
        controller->Server->ChatSendServerMessage(nextMessage.str());
    }
}

void JukeboxPlugin::DisplayMapList(Player player)
{
    std::vector<std::pair<std::string, Map>> mapsVector;
    std::copy(controller->Maps->List.begin(), controller->Maps->List.end(), back_inserter(mapsVector));
    std::sort(mapsVector.begin(), mapsVector.end(), [=](const std::pair<std::string, Map>& a, const std::pair<std::string, Map>& b) { return a.second.Id > b.second.Id; });
    if(controller->Plugins->IsLoaded("Karma", "0.2.0"))
    {
        std::sort(mapsVector.begin(), mapsVector.end(), [=](const std::pair<std::string, Map>& a, const std::pair<std::string, Map>& b) {
            int karmaA = 0;
            int karmaB = 0;

            if(a.second.Additionals.find("Karma") != a.second.Additionals.end())
                karmaA = boost::any_cast<int>(a.second.Additionals.at("Karma"));

            if(b.second.Additionals.find("Karma") != b.second.Additionals.end())
                karmaB = boost::any_cast<int>(b.second.Additionals.at("Karma"));

            return karmaA > karmaB;
        });
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
    list.Actions.insert(std::pair<std::string, std::pair<std::string, std::string>>("Name", std::pair<std::string, std::string>("JukeboxMap", "UId")));

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
                boost::any mapKarma = controller->Plugins->CallMethod("Karma", "GetKarmaByUid", mapInList.UId);
                if(mapKarma.type() == typeid(int))
                {
                    karma = boost::any_cast<int>(mapKarma);
                }
            }

            row.insert(std::pair<std::string, std::string>("Karma", std::to_string(karma)));
        }
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}
