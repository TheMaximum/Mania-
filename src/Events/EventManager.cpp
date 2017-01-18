#include "EventManager.h"

EventManager::EventManager()
{
    methodsEverySecond = std::vector<std::function<void()>>();
    methodsEveryMinute = std::vector<std::function<void()>>();

    methodsPlayerConnect = std::vector<std::function<void(Player)>>();
    methodsPlayerDisconnect = std::vector<std::function<void(Player)>>();
    methodsPlayerChat = std::vector<std::function<void(Player, std::string)>>();
    methodsPlayerManialinkPageAnswer = std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>>();
    methodsEcho = std::vector<std::function<void(std::string, std::string)>> ();
    methodsBeginMatch = std::vector<std::function<void()>>();
    methodsEndMatch = std::vector<std::function<void(std::vector<PlayerRanking>, int)>>();
    methodsBeginMap = std::vector<std::function<void(Map)>>();
    methodsEndMap = std::vector<std::function<void(Map)>>();
    methodsStatusChanged = std::vector<std::function<void(int, std::string)>>();
    methodsPlayerCheckpoint = std::vector<std::function<void(Player, int, int, int)>>();
    methodsPlayerFinish = std::vector<std::function<void(Player, int)>>();
    methodsPlayerIncoherence = std::vector<std::function<void(Player)>>();
    methodsBillUpdated = std::vector<std::function<void(int, int, std::string, int)>>();
    methodsMapListModified = std::vector<std::function<void(int, int, bool)>>();
    methodsPlayerInfoChanged = std::vector<std::function<void(Player)>>();
    methodsVoteUpdated = std::vector<std::function<void(std::string, std::string, std::string, std::string)>>();
}

int EventManager::RegisterEverySecond(std::vector<std::function<void()>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsEverySecond.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterEveryMinute(std::vector<std::function<void()>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsEveryMinute.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
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

int EventManager::RegisterPlayerChat(std::vector<std::function<void(Player, std::string)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerChat.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterPlayerManialinkPageAnswer(std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerManialinkPageAnswer.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterEcho(std::vector<std::function<void(std::string, std::string)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsEcho.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterBeginMatch(std::vector<std::function<void()>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsBeginMatch.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterEndMatch(std::vector<std::function<void(std::vector<PlayerRanking>, int)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsEndMatch.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterBeginMap(std::vector<std::function<void(Map)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsBeginMap.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterEndMap(std::vector<std::function<void(Map)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsEndMap.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterStatusChanged(std::vector<std::function<void(int, std::string)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsStatusChanged.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterPlayerCheckpoint(std::vector<std::function<void(Player, int, int, int)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerCheckpoint.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterPlayerFinish(std::vector<std::function<void(Player, int)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerFinish.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterPlayerIncoherence(std::vector<std::function<void(Player)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerIncoherence.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterBillUpdated(std::vector<std::function<void(int, int, std::string, int)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsBillUpdated.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterMapListModified(std::vector<std::function<void(int, int, bool)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsMapListModified.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterPlayerInfoChanged(std::vector<std::function<void(Player)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsPlayerInfoChanged.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

int EventManager::RegisterVoteUpdated(std::vector<std::function<void(std::string, std::string, std::string, std::string)>> functions)
{
    int functionsAdded = 0;
    for(int functionId = 0; functionId < functions.size(); functionId++)
    {
        methodsVoteUpdated.push_back(functions.at(functionId));
        functionsAdded++;
    }
    return functionsAdded;
}

void EventManager::CallEverySecond()
{
    for(int methodId = 0; methodId < methodsEverySecond.size(); methodId++)
    {
        std::function<void()> method = methodsEverySecond.at(methodId);
        method();
    }
}

void EventManager::CallEveryMinute()
{
    for(int methodId = 0; methodId < methodsEveryMinute.size(); methodId++)
    {
        std::function<void()> method = methodsEveryMinute.at(methodId);
        method();
    }
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

void EventManager::CallPlayerChat(Player player, std::string text, bool isRegisteredCommand)
{
    for(int methodId = 0; methodId < methodsPlayerChat.size(); methodId++)
    {
        std::function<void(Player, std::string)> method = methodsPlayerChat.at(methodId);
        method(player, text);
    }
}

void EventManager::CallPlayerManialinkPageAnswer(Player player, std::string answer, std::vector<EntryVal> entries)
{
    for(int methodId = 0; methodId < methodsPlayerManialinkPageAnswer.size(); methodId++)
    {
        std::function<void(Player, std::string, std::vector<EntryVal>)> method = methodsPlayerManialinkPageAnswer.at(methodId);
        method(player, answer, entries);
    }
}

void EventManager::CallEcho(std::string internal, std::string publicEcho)
{
    for(int methodId = 0; methodId < methodsEcho.size(); methodId++)
    {
        std::function<void(std::string, std::string)> method = methodsEcho.at(methodId);
        method(internal, publicEcho);
    }
}

void EventManager::CallBeginMatch()
{
    for(int methodId = 0; methodId < methodsBeginMatch.size(); methodId++)
    {
        std::function<void()> method = methodsBeginMatch.at(methodId);
        method();
    }
}

void EventManager::CallEndMatch(std::vector<PlayerRanking> rankings, int winnerTeam)
{
    for(int methodId = 0; methodId < methodsEndMatch.size(); methodId++)
    {
        std::function<void(std::vector<PlayerRanking>, int)> method = methodsEndMatch.at(methodId);
        method(rankings, winnerTeam);
    }
}

void EventManager::CallBeginMap(Map map)
{
    for(int methodId = 0; methodId < methodsBeginMap.size(); methodId++)
    {
        std::function<void(Map)> method = methodsBeginMap.at(methodId);
        method(map);
    }
}

void EventManager::CallEndMap(Map map)
{
    for(int methodId = 0; methodId < methodsEndMap.size(); methodId++)
    {
        std::function<void(Map)> method = methodsEndMap.at(methodId);
        method(map);
    }
}

void EventManager::CallStatusChanged(int statusCode, std::string statusName)
{
    for(int methodId = 0; methodId < methodsStatusChanged.size(); methodId++)
    {
        std::function<void(int, std::string)> method = methodsStatusChanged.at(methodId);
        method(statusCode, statusName);
    }
}

void EventManager::CallPlayerCheckpoint(Player player, int time, int currentLap, int checkpointIndex)
{
    for(int methodId = 0; methodId < methodsPlayerCheckpoint.size(); methodId++)
    {
        std::function<void(Player, int, int, int)> method = methodsPlayerCheckpoint.at(methodId);
        method(player, time, currentLap, checkpointIndex);
    }
}

void EventManager::CallPlayerFinish(Player player, int time)
{
    for(int methodId = 0; methodId < methodsPlayerFinish.size(); methodId++)
    {
        std::function<void(Player, int)> method = methodsPlayerFinish.at(methodId);
        method(player, time);
    }
}

void EventManager::CallPlayerIncoherence(Player player)
{
    for(int methodId = 0; methodId < methodsPlayerIncoherence.size(); methodId++)
    {
        std::function<void(Player)> method = methodsPlayerIncoherence.at(methodId);
        method(player);
    }
}

void EventManager::CallBillUpdated(int billId, int state, std::string stateName, int transactionId)
{
    for(int methodId = 0; methodId < methodsBillUpdated.size(); methodId++)
    {
        std::function<void(int, int, std::string, int)> method = methodsBillUpdated.at(methodId);
        method(billId, state, stateName, transactionId);
    }
}

void EventManager::CallMapListModified(int currentMap, int nextMap, bool listModified)
{
    for(int methodId = 0; methodId < methodsMapListModified.size(); methodId++)
    {
        std::function<void(int, int, int)> method = methodsMapListModified.at(methodId);
        method(currentMap, nextMap, listModified);
    }
}

void EventManager::CallPlayerInfoChanged(Player player)
{
    for(int methodId = 0; methodId < methodsPlayerInfoChanged.size(); methodId++)
    {
        std::function<void(Player)> method = methodsPlayerInfoChanged.at(methodId);
        method(player);
    }
}

void EventManager::CallVoteUpdated(std::string stateName, std::string login, std::string commandName, std::string commandParam)
{
    for(int methodId = 0; methodId < methodsVoteUpdated.size(); methodId++)
    {
        std::function<void(std::string, std::string, std::string, std::string)> method = methodsVoteUpdated.at(methodId);
        method(stateName, login, commandName, commandParam);
    }
}
