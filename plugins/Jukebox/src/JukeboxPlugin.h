#ifndef KARMAPLUGIN_H_
#define KARMAPLUGIN_H_

#include <algorithm>
#include <sstream>
#include <map>
#include <vector>

#include "Plugins/Plugin.h"
#include "UI/UIList.h"
#include "Utils/Time.h"

class JukeboxPlugin : public Plugin
{
public:
    JukeboxPlugin();

    void Init();
    void DisplayMapList(Player player);

private:
    void loadSettings();
};

#endif // KARMAPLUGIN_H_
