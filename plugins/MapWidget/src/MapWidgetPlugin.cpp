#include "MapWidgetPlugin.h"

MapWidgetPlugin::MapWidgetPlugin()
{
    Version = "0.1.0";
    Author = "TheM";
    Description = "Displays map information in a widget.";

    BeginMap.push_back([this](Map map) { OnBeginMap(); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    BeginMatch.push_back([this]() { OnBeginMap(); });
    EndMatch.push_back([this](std::vector<PlayerRanking> rankings, int winnerTeam) { OnEndMatch(rankings, winnerTeam); });
}

void MapWidgetPlugin::Init()
{
    loadSettings();
    widget = MapWidget(controller->UI);
    widget.WidgetX = widgetX;
    widget.WidgetY = widgetY;

    if(!widget.DisplayToAll(controller->Players, controller->Maps->Current))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void MapWidgetPlugin::loadSettings()
{
    std::map<std::string, std::string>::iterator widgetXIt = Settings.find("widgetX");
    if(widgetXIt != Settings.end())
        widgetX = atof(widgetXIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetYIt = Settings.find("widgetY");
    if(widgetYIt != Settings.end())
        widgetY = atof(widgetYIt->second.c_str());
}

void MapWidgetPlugin::OnBeginMap()
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
