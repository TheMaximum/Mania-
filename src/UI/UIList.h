#ifndef UILIST_H_
#define UILIST_H_

#include <map>
#include <vector>

//* UILIST
/**
 * \brief ManiaLink for UI list.
 */
struct UIList
{
    std::string Id;                        /**< \brief ManiaLink identifier. */
    std::string Title;                     /**< \brief List title. */
    std::string IconStyle;                 /**< \brief Header icon style. */
    std::string IconSubstyle;              /**< \brief Header icon substyle. */

    std::vector<std::pair<std::string, int>> Columns;     /**< \brief List columns. */
    std::vector<std::map<std::string, std::string>> Rows; /**< \brief List rows. */
    std::map<std::string, std::pair<std::string, std::string>> Actions; /**< \brief List of column/row actions.  */

    UIList()
    {
        Columns = std::vector<std::pair<std::string, int>>();
        Rows = std::vector<std::map<std::string, std::string>>();
        Actions = std::map<std::string, std::pair<std::string, std::string>>();
    }
};

#endif // UILIST_H_
