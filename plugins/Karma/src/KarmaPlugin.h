#ifndef KARMAPLUGIN_H_
#define KARMAPLUGIN_H_

#include <algorithm>
#include <sstream>
#include <map>

#include "Plugins/Plugin.h"
#include "UI/UIList.h"
#include "Utils/Logging.h"
#include "Utils/Time.h"

#include "Objects/MapKarma.h"
#include "Widget/KarmaWidget.h"

class KarmaPlugin : public Plugin
{
public:
    KarmaPlugin();

    void Init();
    void OnBeginMap(Map map);
    void OnPlayerConnect(Player player);
    void OnPlayerChat(Player player, std::string text);

    void DisplayCurrentKarma(Player player);
    void VoteNegative(Player player);
    void VotePositive(Player player);
    void DisplayWhoKarma(Player player);

    boost::any GetKarmaByUid(boost::any parameters);

private:
    std::map<std::string, int> votes;
    MapKarma karma;

    KarmaWidget widget;

    double widgetX = 49.2;
    double widgetY = 39.2;
    int voteAfterFinishes = 0;

    void loadSettings();
    void displayToAll();
    void updateAllMapKarma();
    void updateMapKarma(Map* map, int mapKarma);
    std::map<std::string, int> retrieveVotes(Map map);

    int retrieveTimesDriven(Player player);
};

#endif // KARMAPLUGIN_H_
