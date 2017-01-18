#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <vector>
#include <functional>

#include "Structs.h"
#include "../Objects/Map.h"
#include "../Objects/Player.h"

//* EventManager
/**
 * \brief Handles events (callbacks) from server to plugins.
 */
class EventManager
{
public:
    /*!
     * Simple constructor.
     */
    EventManager();

    /*!
     * \brief Register functions for the EverySecond callback (from controller).
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterEverySecond(std::vector<std::function<void()>> functions);

    /*!
     * \brief Register functions for the EveryMinute callback (from controller).
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterEveryMinute(std::vector<std::function<void()>> functions);

    /*!
     * \brief Register functions for the PlayerConnect callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerConnect(std::vector<std::function<void(Player)>> functions);

    /*!
     * \brief Register functions for the PlayerDisconnect callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerDisconnect(std::vector<std::function<void(Player)>> functions);

    /*!
     * \brief Register functions for the PlayerChat callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerChat(std::vector<std::function<void(Player, std::string)>> functions);

    /*!
     * \brief Register functions for the PlayerManialinkPageAnswer callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerManialinkPageAnswer(std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>> functions);

    /*!
     * \brief Register functions for the Echo callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterEcho(std::vector<std::function<void(std::string, std::string)>> functions);

    /*!
     * \brief Register functions for the BeginMatch callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterBeginMatch(std::vector<std::function<void()>> functions);

    /*!
     * \brief Register functions for the EndMatch callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterEndMatch(std::vector<std::function<void(std::vector<PlayerRanking>, int)>> functions);

    /*!
     * \brief Register functions for the BeginMap callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterBeginMap(std::vector<std::function<void(Map)>> functions);

    /*!
     * \brief Register functions for the EndMap callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterEndMap(std::vector<std::function<void(Map)>> functions);

    /*!
     * \brief Register functions for the StatusChanged callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterStatusChanged(std::vector<std::function<void(int, std::string)>> functions);

    /*!
     * \brief Register functions for the PlayerCheckpoint callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerCheckpoint(std::vector<std::function<void(Player, int, int, int)>> functions);

    /*!
     * \brief Register functions for the PlayerFinish callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerFinish(std::vector<std::function<void(Player, int)>> functions);

    /*!
     * \brief Register functions for the PlayerIncoherence callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerIncoherence(std::vector<std::function<void(Player)>> functions);

    /*!
     * \brief Register functions for the BillUpdated callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterBillUpdated(std::vector<std::function<void(int, int, std::string, int)>> functions);

    /*!
     * \brief Register functions for the MapListModified callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterMapListModified(std::vector<std::function<void(int, int, bool)>> functions);

    /*!
     * \brief Register functions for the PlayerInfoChanged callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerInfoChanged(std::vector<std::function<void(Player)>> functions);

    /*!
     * \brief Register functions for the VoteUpdated callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterVoteUpdated(std::vector<std::function<void(std::string, std::string, std::string, std::string)>> functions);

    /*!
     * \brief Calls all functions which are subscribed to the EverySecond event.
     */
    void CallEverySecond();

    /*!
     * \brief Calls all functions which are subscribed to the EveryMinute event.
     */
    void CallEveryMinute();

    /*!
     * \brief Calls all functions which are subscribed to the PlayerConnect event.
     *
     * \param player Player that connected.
     */
    void CallPlayerConnect(Player player);

    /*!
     * \brief Calls all functions which are subscribed to the PlayerDisconnect event.
     *
     * \param player Player that disconnected.
     */
    void CallPlayerDisconnect(Player player);

    /*!
     * \brief Calls all functions which are subscribed to the PlayerChat event.
     *
     * \param player              Player that wrote a chat message.
     * \param text                Chat message.
     * \param isRegisteredCommand Is the chat message a registered command?
     */
    void CallPlayerChat(Player player, std::string text, bool isRegisteredCommand);

    /*!
     * \brief Calls all functions which are subscribed to the PlayerManialinkPageAnswer event.
     *
     * \param player              Player who created the answer.
     * \param answer              ManiaLink answer.
     * \param entries             List of entries.
     */
    void CallPlayerManialinkPageAnswer(Player player, std::string answer, std::vector<EntryVal> entries);

    /*!
     * \brief Calls all functions which are subscribed to the Echo event.
     *
     * \param internal            Internal.
     * \param publicEcho          Public.
     */
    void CallEcho(std::string internal, std::string publicEcho);

    /*!
     * \brief Calls all functions which are subscribed to the BeginMatch event.
     */
    void CallBeginMatch();

    /*!
     * \brief Calls all functions which are subscribed to the EndMatch event.
     *
     * \param rankings            Player rankings.
     * \param winnerTeam          Winning team.
     */
    void CallEndMatch(std::vector<PlayerRanking> rankings, int winnerTeam);

    /*!
     * \brief Calls all functions which are subscribed to the EndMatch event.
     *
     * \param map                 New current map.
     */
    void CallBeginMap(Map map);

    /*!
     * \brief Calls all functions which are subscribed to the EndMap event.
     *
     * \param map                 New current map.
     */
    void CallEndMap(Map map);

    /*!
     * \brief Calls all functions which are subscribed to the StatusChanged event.
     *
     * \param statusCode          New status code.
     * \param statusName          New status name.
     */
    void CallStatusChanged(int statusCode, std::string statusName);

    /*!
     * \brief Calls all functions which are subscribed to the PlayerCheckpoint event.
     *
     * \param player              Player object.
     * \param time                Checkpoint time.
     * \param currentLap          Current lap number.
     * \param checkpointIndex     Current checkpoint number.
     */
    void CallPlayerCheckpoint(Player player, int time, int currentLap, int checkpointIndex);

    /*!
     * \brief Calls all functions which are subscribed to the PlayerFinish event.
     *
     * \param player              Player object.
     * \param time                Finish time.
     */
    void CallPlayerFinish(Player player, int time);

    /*!
     * \brief Calls all functions which are subscribed to the PlayerIncoherence event.
     *
     * \param player              Player object.
     */
    void CallPlayerIncoherence(Player player);

    /*!
     * \brief Calls all functions which are subscribed to the BillUpdated event.
     *
     * \param billId              Bill identifier.
     * \param state               New bill state.
     * \param stateName           New bill state name.
     * \param transactionId       Transaction identifier.
     */
    void CallBillUpdated(int billId, int state, std::string stateName, int transactionId);

    /*!
     * \brief Calls all functions which are subscribed to the MapListModified event.
     *
     * \param currentMap          Current map index.
     * \param nextMap             Next map index.
     * \param listModified        Is maplist modified?
     */
    void CallMapListModified(int currentMap, int nextMap, bool listModified);

    /*!
     * \brief Calls all functions which are subscribed to the PlayerInfoChanged event.
     *
     * \param player              Player object.
     */
    void CallPlayerInfoChanged(Player player);

    /*!
     * \brief Calls all functions which are subscribed to the VoteUpdated event.
     *
     * \param stateName           Vote state name.
     * \param login               Player login. (?)
     * \param commandName         Name of command.
     * \param commandParam        Parameter for command.
     */
    void CallVoteUpdated(std::string stateName, std::string login, std::string commandName, std::string commandParam);

private:
    std::vector<std::function<void()>> methodsEverySecond;                             /**< \brief Vector with functions for the EverySecond event. */
    std::vector<std::function<void()>> methodsEveryMinute;                             /**< \brief Vector with functions for the EveryMinute event. */

    std::vector<std::function<void(Player)>> methodsPlayerConnect;                     /**< \brief Vector with functions for the PlayerConnect event. */
    std::vector<std::function<void(Player)>> methodsPlayerDisconnect;                  /**< \brief Vector with functions for the PlayerDisconnect event. */
    std::vector<std::function<void(Player, std::string)>> methodsPlayerChat;           /**< \brief Vector with functions for the PlayerChat event. */
    std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>> methodsPlayerManialinkPageAnswer; /**< \brief Vector with functions for the PlayerManialinkPageAnswer event. */
    std::vector<std::function<void(std::string, std::string)>> methodsEcho;            /**< \brief Vector with functions for the Echo event. */
    std::vector<std::function<void()>> methodsBeginMatch;                              /**< \brief Vector with functions for the BeginMatch event. */
    std::vector<std::function<void(std::vector<PlayerRanking>, int)>> methodsEndMatch; /**< \brief Vector with functions for the EndMatch event. */
    std::vector<std::function<void(Map)>> methodsBeginMap;                             /**< \brief Vector with functions for the BeginMap event. */
    std::vector<std::function<void(Map)>> methodsEndMap;                               /**< \brief Vector with functions for the EndMap event. */
    std::vector<std::function<void(int, std::string)>> methodsStatusChanged;           /**< \brief Vector with functions for the StatusChanged event. */
    std::vector<std::function<void(Player, int, int, int)>> methodsPlayerCheckpoint;   /**< \brief Vector with functions for the PlayerCheckpoint event. */
    std::vector<std::function<void(Player, int)>> methodsPlayerFinish;                 /**< \brief Vector with functions for the PlayerFinish event. */
    std::vector<std::function<void(Player)>> methodsPlayerIncoherence;                 /**< \brief Vector with functions for the PlayerIncoherence event. */
    std::vector<std::function<void(int, int, std::string, int)>> methodsBillUpdated;   /**< \brief Vector with functions for the BillUpdated event. */
    std::vector<std::function<void(int, int, bool)>> methodsMapListModified;           /**< \brief Vector with functions for the MapListModified event. */
    std::vector<std::function<void(Player)>> methodsPlayerInfoChanged;                 /**< \brief Vector with functions for the PlayerInfoChanged event. */
    std::vector<std::function<void(std::string, std::string, std::string, std::string)>> methodsVoteUpdated; /**< \brief Vector with functions for the VoteUpdated event. */
};

#endif // EVENTMANAGER_H_
