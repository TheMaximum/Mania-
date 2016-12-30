#ifndef CALLBACKMANAGER_H_
#define CALLBACKMANAGER_H_

#include <string>
#include <map>
#include <vector>

#include "../Events/EventManager.h"
#include "../GbxRemote/GbxRemote.h"
#include "../GbxRemote/Message/GbxMessage.h"
#include "../GbxRemote/Parameters/GbxParameters.h"
#include "../Objects/Map.h"
#include "../Objects/Player.h"
#include "../Utils/Logging.h"

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
     * \param serverPtr       Pointer to the GbxRemote.
     * \param eventManagerPtr Pointer to the EventManager.
     * \param playerList      Pointer to playerlist.
     * \param mapList         Pointer to maplist.
     */
    CallBackManager(GbxRemote* serverPtr, EventManager* eventManagerPtr, std::map<std::string, Player>* playerList, std::map<std::string, Map>* mapList);

    /*!
     * \brief Handles callback (updates lists, calls plugin functions).
     *
     * \param methodName Name of the callback method.
     * \param parameters List with method parameters.
     */
    void HandleCallBack(std::string methodName, std::vector<GbxResponseParameter> parameters);

private:
    GbxRemote* server;                      /**< \brief Contains the serverconnection. */
    EventManager* events;                   /**< \brief Contains the event manager. */
    std::map<std::string, Player>* players; /**< \brief Contains the list of players currently on the server. */
    std::map<std::string, Map>* maps;       /**< \brief Contains the list of players currently on the server. */
};

#endif // CALLBACKMANAGER_H_
