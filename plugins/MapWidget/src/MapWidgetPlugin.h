#ifndef MAPWIDGETPLUGIN_H_
#define MAPWIDGETPLUGIN_H_

#include <sstream>
#include "Plugins/Plugin.h"
#include "Utils/Time.h"

#include "Widget/MapWidget.h"

class MapWidgetPlugin : public Plugin
{
public:
    MapWidgetPlugin();

    void Init();
    void OnBeginMap(Map map);
    void OnPlayerConnect(Player player);
    void OnEndMatch(std::vector<PlayerRanking> rankings, int winnerTeam);

private:
    MapWidget widget;
};

#endif // MAPWIDGETPLUGIN_H_
