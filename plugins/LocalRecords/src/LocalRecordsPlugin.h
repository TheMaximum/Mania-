#ifndef LOCALRECORDSPLUGIN_H_
#define LOCALRECORDSPLUGIN_H_

#include <sstream>
#include "Plugins/Plugin.h"
#include "Utils/Time.h"

#include "Objects/LocalRecord.h"
#include "Widget/LocalRecordsWidget.h"

class LocalRecordsPlugin : public Plugin
{
public:
    LocalRecordsPlugin();

    void Init();
    void OnBeginMap(Map map);
    void OpenLocalRecords(Player player, std::string answer, std::vector<EntryVal> entries);

private:
    std::vector<LocalRecord> localRecords;
    LocalRecordsWidget widget;

    void retrieveRecords(Map map);
};

#endif // LOCALRECORDSPLUGIN_H_
