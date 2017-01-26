#include "JukeboxPlugin.h"

JukeboxPlugin::JukeboxPlugin()
{
    Version = "0.2.0";
    Author = "TheM";
    Description = "Offers map list functionality and the possibility for players to decide what to play next.";

    EndMatch.push_back([this](std::vector<PlayerRanking> rankings, int winnerTeam) { OnEndMatch(); });
    RegisterCommand("list", "Display list of maps on the server.", [this](Player player, std::vector<std::string> parameters) { DisplayMapList(player, parameters); });
    RegisterCommand("jukebox", "[list] = open current jukebox, [drop] = drop your jukeboxed map.", [this](Player player, std::vector<std::string> parameters) { ChatJukebox(player, parameters); });
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
            DisplayJukeboxList(player);
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
