#ifndef CALLBACKMANAGER_H_
#define CALLBACKMANAGER_H_

#include <ctime>
#include <string>
#include <map>
#include <vector>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "../Config/Config.h"
#include "../Commands/CommandManager.h"
#include "../Events/EventManager.h"
#include "../GbxRemote/GbxRemote.h"
#include "../GbxRemote/Message/GbxMessage.h"
#include "../GbxRemote/Parameters/GbxParameters.h"
#include "../Maps/MapList.h"
#include "../Methods/Structs.h"
#include "../Objects/Map.h"
#include "../Objects/Player.h"
#include "../Utils/Logging.h"
#include "../Utils/Time.h"

//* CallBackManager
/**
 * \brief Handles callbacks from the server.
 */
class CallBackManager
{
public:
    /*!
     * Constructor takes pointers to playerlist and maplist to keep them updated.
     *
     * \todo Replace pointer to GbxRemote with pointer to Methods.
     *
     * \param configPtr         Pointer to the Config.
     * \param serverPtr         Pointer to the GbxRemote.
     * \param commandManagerPtr Pointer to the CommandManager.
     * \param eventManagerPtr   Pointer to the EventManager.
     * \param databasePtr       Pointer to the database connection.
     * \param playerList        Pointer to playerlist.
     * \param mapList           Pointer to maplist.
     * \param serverInfoPtr     Pointer to server information.
     */
    CallBackManager(Config* configPtr, GbxRemote* serverPtr, CommandManager* commandManagerPtr, EventManager* eventManagerPtr, sql::Connection* databasePtr, std::map<std::string, Player>* playerList, MapList* mapList, ServerInfo* serverInfoPtr);

    /*!
     * \brief Handles callback (updates lists, calls plugin functions).
     *
     * \param methodName Name of the callback method.
     * \param parameters List with method parameters.
     */
    void HandleCallBack(std::string methodName, std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerConnect callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerConnect(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerDisconnect callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerDisconnect(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerChat callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerChat(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerManialinkPageAnswer callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerManialinkPageAnswer(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles Echo callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleEcho(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles BeginMatch callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleBeginMatch(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles EndMatch callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleEndMatch(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles BeginMap callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleBeginMap(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles EndMap callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleEndMap(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles StatusChanged callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleStatusChanged(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerCheckpoint callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerCheckpoint(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerFinish callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerFinish(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerIncoherence callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerIncoherence(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles BillUpdated callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleBillUpdated(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles MapListModified callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleMapListModified(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles PlayerInfoChanged callback.
     *
     * \param parameters List with method parameters.
     */
    void HandlePlayerInfoChanged(std::vector<GbxResponseParameter> parameters);

    /*!
     * \brief Handles VoteUpdated callback.
     *
     * \param parameters List with method parameters.
     */
    void HandleVoteUpdated(std::vector<GbxResponseParameter> parameters);

private:
    Config* config;                         /**< \brief Contains the configuration. */
    GbxRemote* server;                      /**< \brief Contains the serverconnection. */
    CommandManager* commands;               /**< \brief Contains the command manager. */
    EventManager* events;                   /**< \brief Contains the event manager. */
    sql::Connection* database;              /**< \brief Contains the database connection. */
    std::map<std::string, Player>* players; /**< \brief Contains the list of players currently on the server. */
    MapList* maps;                          /**< \brief Contains the list of players currently on the server. */
    ServerInfo* serverInfo;                 /**< \brief Contains the server information. */
};

#endif // CALLBACKMANAGER_H_
