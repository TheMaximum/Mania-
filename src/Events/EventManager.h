#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <vector>
#include <functional>

#include "../Objects/Player.h"

class EventManager
{
public:
    EventManager();

    int RegisterPlayerConnect(std::vector<std::function<void(Player)>> functions);
    int RegisterPlayerDisconnect(std::vector<std::function<void(Player)>> functions);

    void CallPlayerConnect(Player player);
    void CallPlayerDisconnect(Player player);

private:
    std::vector<std::function<void(Player)>> methodsPlayerConnect;
    std::vector<std::function<void(Player)>> methodsPlayerDisconnect;
};

#endif // EVENTMANAGER_H_
