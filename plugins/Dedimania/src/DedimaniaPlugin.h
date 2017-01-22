#ifndef DEDIMANIAPLUGIN_H_
#define DEDIMANIAPLUGIN_H_

#include <algorithm>
#include <sstream>
#include <curl/curl.h>
#include "base64.h"

#include "GbxRemote/Message/GbxMessage.h"
#include "Plugins/Plugin.h"

#include "Utils/Formatting.h"
#include "Utils/GameMode.h"
#include "Utils/GZip.h"
#include "Utils/Parameter.h"
#include "Utils/Time.h"

#include "Objects/DediRecord.h"
#include "Widget/DedimaniaWidget.h"

class DedimaniaPlugin : public Plugin
{
public:
    DedimaniaPlugin();
    void Init();
    void OnBeginMap();
    void OnPlayerConnect(Player player);
    void OnPlayerFinish(Player player, int time);
    void OnEndMatch(std::vector<PlayerRanking> rankings);
    void UpdateServer();

    void OpenDediRecords(Player player);

    static size_t receiveData(void* contents, size_t size, size_t nmemb, std::string* s);

private:
    CURL* curl = NULL;
    bool hasError = false;
    std::string dedimaniaCode = "";
    std::string sessionId = "";
    int maxRank = 0;
    int updateServer = 0;
    bool mapValid = false;

    std::vector<GbxStructParameters> currentCalls = std::vector<GbxStructParameters>();

    std::vector<DediRecord> records = std::vector<DediRecord>();
    std::vector<DediRecord> newRecords = std::vector<DediRecord>();
    DedimaniaWidget widget;

    int widgetEntries = 28;
    int widgetTopCount = 5;
    double widgetX = -64.7;
    double widgetY = 28.2;

    void loadSettings();
    void authenticate();
    GbxResponse multicall();
    GbxResponse query(GbxMessage message);
};

#endif // DEDIMANIAPLUGIN_H_
