#include "MapWidgetPlugin.h"

MapWidgetPlugin::MapWidgetPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    EndMatch.push_back([this](std::vector<PlayerRanking> rankings, int winnerTeam) { OnEndMatch(rankings, winnerTeam); });
}

void MapWidgetPlugin::Init()
{
    widget = MapWidget(controller->UI);

    if(!widget.DisplayToAll(controller->Players, controller->Maps->Current))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void MapWidgetPlugin::OnBeginMap(Map map)
{
    widget.Title = "Current Map";
    if(!widget.DisplayToAll(controller->Players, controller->Maps->Current))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void MapWidgetPlugin::OnEndMatch(std::vector<PlayerRanking> rankings, int winnerTeam)
{
    widget.Title = "Next Map";
    Map nextMap = controller->Server->GetNextMapInfo();
    if(!widget.DisplayToAll(controller->Players, &nextMap))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void MapWidgetPlugin::OnPlayerConnect(Player player)
{
    if(!widget.DisplayToPlayer(player, controller->Maps->Current))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}
