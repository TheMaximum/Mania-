#ifndef HELLOGOODBYEPLUGIN_H_
#define HELLOGOODBYEPLUGIN_H_

#include "../../../src/Plugins/Plugin.h"
#include "../../../src/ManiaPP.h"

class HelloGoodbyePlugin : public Plugin
{
public:
    HelloGoodbyePlugin();

    void Init();
    void OnPlayerConnect(Player player);
    void OnPlayerDisconnect(Player player);
};

#endif // HELLOGOODBYEPLUGIN_H_
