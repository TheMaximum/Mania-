#ifndef KARMAPLUGIN_H_
#define KARMAPLUGIN_H_

#include <sstream>
#include <map>
#include "Plugins/Plugin.h"
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

    void VotePositiveAction(Player player, std::string answer, std::vector<EntryVal> entries);
    void VoteNegativeAction(Player player, std::string answer, std::vector<EntryVal> entries);

    void DisplayKarma(Player player, std::vector<std::string> parameters);
    void VotePositiveChat(Player player, std::vector<std::string> parameters);
    void VoteNegativeChat(Player player, std::vector<std::string> parameters);

private:
    std::map<std::string, int> votes;
    MapKarma karma;

    KarmaWidget widget;

    double widgetX = 49.2;
    double widgetY = 39.2;
    int voteAfterFinishes = 0;

    void loadSettings();
    void displayToAll();
    void retrieveVotes(Map map);
    void displayCurrentKarma(Player player);

    void voteNegative(Player player);
    void votePositive(Player player);

    int retrieveTimesDriven(Player player);
};

#endif // KARMAPLUGIN_H_
