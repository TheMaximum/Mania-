#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include <cmath>

#include "../Events/EventManager.h"
#include "../Methods/Methods.h"
#include "../Utils/Text.h"

#include "UIFrame.h"
#include "UIList.h"

//* UIManager
/**
 * \brief Manages the user interface on the server (ManiaLinks).
 */
class UIManager
{
public:
    /*!
     * \brief Constructor takes needed controller pointers.
     *
     * \param serverPtr  Pointer to the server methods.
     * \param eventsPtr  Pointer to the event manager.
     * \param playersPtr Pointer to the player list.
     */
    UIManager(Methods* serverPtr, EventManager* eventsPtr, std::map<std::string, Player>* playersPtr);

    /*!
     * \brief Callback handler voor ManiaLink page answers.
     *
     * \param player     Player who interacted with the page.
     * \param answer     Page answer.
     * \param entries    Entries.
     */
    void OnPlayerManialinkPageAnswer(Player player, std::string answer, std::vector<EntryVal> entries);

    /*!
     * \brief Display frame.
     *
     * \param frame      Frame to be displayed.
     */
    bool Display(UIFrame frame);

    /*!
     * \brief Display frame to player.
     *
     * \param frame      Frame to be displayed.
     * \param player     Player who should receive the frame.
     */
    bool Display(UIFrame frame, Player player);

    /*!
     * \brief Display list to player.
     *
     * \param list       List to be displayed.
     * \param player     Player who should receive the list.
     */
    bool DisplayList(UIList list, Player player, int currentPage = 1);

    /*!
     * \brief Close list for player.
     *
     * \param player     Player who should receive the list.
     */
    void CloseList(Player player);

    /*!
     * \brief Event listener for EndMatch, closes center lists.
     */
    void OnEndMatch();

    /*!
     * \brief Register event to be handled on page answer.
     *
     * \param answer     Name of the answer.
     * \param function   Function to be called on callback.
     */
    bool RegisterEvent(std::string answer, std::function<void(Player, std::string, std::vector<EntryVal>)> function);

private:
    Methods* server;                            /**< \brief Pointer to the server methods. */
    EventManager* events;                       /**< \brief Pointer to the event manager. */
    std::map<std::string, Player>* players;     /**< \brief Pointer to the player list. */
    std::map<std::string, std::function<void(Player, std::string, std::vector<EntryVal>)>> answers; /**< \brief List with ManiaLink answers to handle. */

    std::map<std::string, UIList> currentLists; /**< \brief Current lists displayed to players. */

    /*!
     * \brief Formats the page, including custom UI.
     *
     * \param frame     Frame to be formatted.
     */
    std::string formatPage(UIFrame frame);
};

#endif // UIMANAGER_H_
