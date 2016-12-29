#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <vector>
#include <functional>

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
     * \brief Register functions for the ManiaPlanet.PlayerConnect callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerConnect(std::vector<std::function<void(Player)>> functions);

    /*!
     * \brief Register functions for the ManiaPlanet.PlayerDisconnect callback.
     *
     * Returns how many functions were successfully added.
     *
     * \param functions Vector with functions to be added.
     */
    int RegisterPlayerDisconnect(std::vector<std::function<void(Player)>> functions);

    /*!
     * Calls all functions which are subscribed to the PlayerConnect event.
     */
    void CallPlayerConnect(Player player);

    /*!
     * Calls all functions which are subscribed to the PlayerDisconnect event.
     */
    void CallPlayerDisconnect(Player player);

private:
    std::vector<std::function<void(Player)>> methodsPlayerConnect;    /**< \brief Vector with PlayerConnect-functions. */
    std::vector<std::function<void(Player)>> methodsPlayerDisconnect; /**< \brief Vector with PlayerDisconnect-functions. */
};

#endif // EVENTMANAGER_H_
