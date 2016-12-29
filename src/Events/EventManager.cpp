#include "EventManager.h"

EventManager::EventManager()
{
    methodsPlayerConnect = std::vector<std::function<void(Player)>>();
    methodsPlayerDisconnect = std::vector<std::function<void(Player)>>();
}

int EventManager::RegisterPlayerConnect(std::vector<std::function<void(Player)>> functions)
{
    int functionsAdded = 0;

    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerConnect.push_back(functions.at(functionId));
        functionsAdded++;
    }

    return functionsAdded;
}

int EventManager::RegisterPlayerDisconnect(std::vector<std::function<void(Player)>> functions)
{
    int functionsAdded = 0;

    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerDisconnect.push_back(functions.at(functionId));
        functionsAdded++;
    }

    return functionsAdded;
}

void EventManager::CallPlayerConnect(Player player)
{
    for(int methodId = 0; methodId < methodsPlayerConnect.size(); methodId++)
    {
        std::function<void(Player)> method = methodsPlayerConnect.at(methodId);
        method(player);
    }
}

void EventManager::CallPlayerDisconnect(Player player)
{
    for(int methodId = 0; methodId < methodsPlayerDisconnect.size(); methodId++)
    {
        std::function<void(Player)> method = methodsPlayerDisconnect.at(methodId);
        method(player);
    }
}
