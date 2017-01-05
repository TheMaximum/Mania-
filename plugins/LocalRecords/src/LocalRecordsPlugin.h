#ifndef LOCALRECORDSPLUGIN_H_
#define LOCALRECORDSPLUGIN_H_

#include <sstream>

#include "Plugins/Plugin.h"
#include "Utils/Time.h"

#include "Objects/LocalRecord.h"
#include "LocalRecordsList.h"
#include "Widget/LocalRecordsWidget.h"

class LocalRecordsPlugin : public Plugin
{
public:
    LocalRecordsPlugin();

    void Init();
    void OnBeginMap(Map map);
    void OnPlayerConnect(Player player);
    void OnPlayerFinish(Player player, int time);
    void OnPlayerCheckpoint(Player player, int time, int currentLap, int checkpointIndex);

    void OpenLocalRecordsAnswer(Player player, std::string answer, std::vector<EntryVal> entries);
    void OpenLocalRecordsCommand(Player player, std::vector<std::string> parameters);

private:
    LocalRecordsList localRecords;
    LocalRecordsWidget widget;

    int recordLimit = 100;
    int widgetEntries = 28;
    int widgetTopCount = 5;
    double widgetX = 49.2;
    double widgetY = 28.2;

    void retrieveRecords(Map map);
    void displayPersonalRecord(Player player);
    void loadSettings();
};

#endif // LOCALRECORDSPLUGIN_H_
