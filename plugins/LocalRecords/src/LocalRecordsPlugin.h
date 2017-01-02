#ifndef LOCALRECORDSPLUGIN_H_
#define LOCALRECORDSPLUGIN_H_

#include <sstream>
#include "Plugins/Plugin.h"
#include "Utils/Time.h"

#include "Objects/LocalRecord.h"

class LocalRecordsPlugin : public Plugin
{
public:
    LocalRecordsPlugin();

    void Init();
    void OnBeginMap(Map map);

private:
    std::vector<LocalRecord> localRecords;

    void retrieveRecords(Map map);
};

#endif // LOCALRECORDSPLUGIN_H_
