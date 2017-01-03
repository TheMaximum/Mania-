#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "../Events/EventManager.h"
#include "../Methods/Methods.h"
#include "UIFrame.h"

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
     * \param serverPtr Pointer to the server methods.
     * \param eventsPtr Pointer to the event manager.
     */
    UIManager(Methods* serverPtr, EventManager* eventsPtr);

    /*!
     * \brief Callback handler voor ManiaLink page answers.
     *
     * \param player    Player who interacted with the page.
     * \param answer    Page answer.
     * \param entries   Entries.
     */
    void OnPlayerManialinkPageAnswer(Player player, std::string answer, std::vector<EntryVal> entries);

    /*!
     * \brief Display frame.
     *
     * \param frame     Frame to be displayed.
     */
    bool Display(UIFrame frame);

    /*!
     * \brief Display frame to player.
     *
     * \param frame     Frame to be displayed.
     * \param player    Player who should receive the frame.
     */
    bool Display(UIFrame frame, Player player);

    /*!
     * \brief Add event to be handled on page answer.
     *
     * \param answer    Name of the answer.
     * \param function  Function to be called on callback.
     */
    bool AddEvent(std::string answer, std::function<void(Player, std::string, std::vector<EntryVal>)> function);

private:
    Methods* server;      /**< \brief Pointer to the server methods. */
    EventManager* events; /**< \brief Pointer to the event manager. */
    std::map<std::string, std::function<void(Player, std::string, std::vector<EntryVal>)>> answers; /**< \brief List with ManiaLink answers to handle. */
};

#endif // UIMANAGER_H_
