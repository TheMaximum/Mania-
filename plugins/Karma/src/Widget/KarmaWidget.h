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

#include "../Objects/MapKarma.h"

class KarmaWidget
{
public:
    KarmaWidget();
    KarmaWidget(UIManager* uiManager);
    bool DisplayToPlayer(Player player, MapKarma* karma, int personalScore);

    double WidgetX = 49.2;
    double WidgetY = 39.2;

    std::string ActionId = "OpenWhoKarma";
    std::string PositiveAction = "KarmaVotePositive";
    std::string NegativeAction = "KarmaVoteNegative";

private:
    UIManager* ui;
    UIFrame frame;

    std::string title = "Karma";
    std::string manialinkId = "Karma";
};

#endif // MAPWIDGET_H_
