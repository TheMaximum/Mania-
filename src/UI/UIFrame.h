#ifndef UIFRAME_H_
#define UIFRAME_H_

#include <functional>
#include <map>
#include "../Events/Structs.h"

//* UIFrame
/**
 * \brief ManiaLink for UI.
 */
struct UIFrame
{
    std::string ManiaLinkId; /**< \brief ManiaLink identifier. */
    std::string Page;        /**< \brief XML ManiaLink page. */
    int Timeout;             /**< \brief Timeout of the page. */
    bool CloseOnClick;       /**< \brief Close the page when being clicked. */
};

#endif // UIFRAME_H_
