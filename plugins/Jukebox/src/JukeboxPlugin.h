#ifndef KARMAPLUGIN_H_
#define KARMAPLUGIN_H_

#include <algorithm>
#include <sstream>
#include <map>
#include <vector>

#include "Plugins/Plugin.h"
#include "UI/UIList.h"
#include "Utils/Time.h"

struct JukeboxItem
{
    Map map;
    Player player;
};

class JukeboxPlugin : public Plugin
{
public:
    JukeboxPlugin();

    void Init();
    void OnEndMatch();
    void ChatJukebox(Player player, std::vector<std::string> parameters);
    void JukeboxMap(Player player, std::string answer);
    void DisplayJukeboxList(Player player);
    void DisplayMapList(Player player, std::vector<std::string> parameters);

private:
    void loadSettings();

    std::vector<JukeboxItem> jukebox;
    bool skipMapWhenLeft = true;
};

#endif // KARMAPLUGIN_H_
