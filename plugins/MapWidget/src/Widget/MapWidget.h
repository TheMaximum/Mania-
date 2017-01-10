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

    double WidgetX = 49.2;
    double WidgetY = 48.2;

private:
    UIManager* ui;
    UIFrame frame;

    std::string manialinkId = "MapInfo";
};

#endif // MAPWIDGET_H_
