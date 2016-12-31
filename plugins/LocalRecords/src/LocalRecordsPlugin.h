#ifndef LOCALRECORDSPLUGIN_H_
#define LOCALRECORDSPLUGIN_H_

#include <sstream>
#include "Plugins/Plugin.h"

class LocalRecordsPlugin : public Plugin
{
public:
    LocalRecordsPlugin();

    void Init();
    void OnBeginMap(Map map);
};

#endif // LOCALRECORDSPLUGIN_H_
