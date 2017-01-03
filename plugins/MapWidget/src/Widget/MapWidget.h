#ifndef MAPWIDGET_H_
#define MAPWIDGET_H_

#include <string>
#include <vector>
#include <map>

#include "Methods/Methods.h"
#include "Objects/Map.h"
#include "Objects/Player.h"
#include "UI/UIManager.h"
#include "Utils/Logging.h"
#include "Utils/Time.h"

class MapWidget
{
public:
    MapWidget();
    MapWidget(UIManager* uiManager);
    bool DisplayToAll(std::map<std::string, Player>* players, Map* currentMap);
    bool DisplayToPlayer(Player player, Map* currentMap);

    std::string Title = "Current Map";

private:
    UIManager* ui;

    UIFrame frame;

    std::string manialinkId = "MapInfo";
    //std::string actionId = "OpenMapInfo";

    double widgetWidth = 15.5;
    double widgetHeight = 8.9;
    double columnHeight = (widgetHeight - 3.1);
    double backgroundWidth = (widgetWidth - 0.2);
    double backgroundHeight = (widgetHeight - 0.2);
    double borderWidth = (widgetWidth + 0.4);
    double borderHeight = (widgetHeight + 0.6);

    double widgetX = 49.2;
    double widgetY = 48.2;

    double left_IconX = 0.6;
    double left_IconY = 0;
    double left_TitleX = 3.2;
    double left_TitleY = -0.65;
    std::string left_TitleHalign = "left";
    double left_ImageOpenX = -0.3;
    std::string left_ImageOpen = "http://static.undef.name/ingame/records-eyepiece/edge-open-ld-dark.png";

    double right_IconX = 12.5;
    double right_IconY = 0;
    double right_TitleX = 12.4;
    double right_TitleY = -0.65;
    std::string right_TitleHalign = "right";
    double right_ImageOpenX = 12.2;
    std::string right_ImageOpen = "http://static.undef.name/ingame/records-eyepiece/edge-open-rd-dark.png";

    std::string backgroundColor = "3342";
    std::string backgroundFocus = "09F6";

    std::string backgroundStyle = "Bgs1";
    std::string backgroundSubstyle = "BgTitleGlow";
    std::string borderStyle = "Bgs1";
    std::string borderSubstyle = "BgTitleShadow";

    double imageOpenX = (widgetX < 0) ? (right_ImageOpenX + (widgetWidth - 15.5)) : left_ImageOpenX;
    double imageOpenY = -(widgetHeight - 3.18);
    std::string imageOpen = (widgetX < 0) ? right_ImageOpen : left_ImageOpen;

    double titleBackgroundWidth = (widgetWidth - 0.8);
    std::string titleStyle = "BgsPlayerCard";
    std::string titleSubstyle = "BgRacePlayerName";
    double titleX = (widgetX < 0) ? (right_TitleX + (widgetWidth - 15.5)) : left_TitleX;
    double titleY = (widgetX < 0) ? right_TitleY : left_TitleY;
    std::string titleHalign = (widgetX < 0) ? right_TitleHalign : left_TitleHalign;

    double iconX = (widgetX < 0) ? (right_IconX + (widgetWidth - 15.5)) : left_IconX;
    double iconY = (widgetX < 0) ? right_IconY : left_IconY;
    std::string iconStyle = "Icons128x128_1";
    std::string iconSubstyle = "Challenge";

    std::string textColor = "FFFF";
};

#endif // MAPWIDGET_H_
