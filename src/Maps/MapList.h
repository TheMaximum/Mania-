#ifndef MAPLIST_H_
#define MAPLIST_H_

#include "../Objects/Map.h"

//* MapList
/**
 * \brief Contains the maplist and a pointer to the current map.
 */
class MapList
{
public:
    std::map<std::string, Map> List; /**< \brief List of all maps on the server. */
    Map* Current = NULL;             /**< \brief Pointer to the current map (in the list). */

    /*!
     * \brief Sets the current map to the one belonging to the unique identifier.
     *
     * \param uid Map identifier (from game/server).
     */
    void SetCurrentMap(std::string uid);

    /*!
     * \brief Gets map for unique identifier.
     *
     * \param uid Map identifier (from game/server).
     */
    Map* GetByUid(std::string uid);
};

#endif // MAPLIST_H_
