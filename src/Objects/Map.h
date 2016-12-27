#ifndef MAP_H_
#define MAP_H_

#include "../GbxRemote/Response/GbxResponse.h"

//* Map
/**
 * \brief Contains all information about a map in easy-to-use format.
 */
struct Map
{
    std::string UId;          /**< \brief Unique map identifier. */
    std::string Name;         /**< \brief Map name. */
    std::string FileName;     /**< \brief File name. */
    std::string Environment;  /**< \brief Game environment.*/
    std::string Author;       /**< \brief Login of the author.*/

    int GoldTime;             /**< \brief Time of the gold medal. */
    int CopperPrice;          /**< \brief Copper price. */

    std::string MapType;      /**< \brief Type of the map. */
    std::string MapStyle;     /**< \brief Map style (self-entered). */

    /*!
     * \brief Formats a server response into a usable form.
     *
     * \param serverStruct The struct with map information received from the server.
     */
    Map(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        if(serverStruct.find("UId") != serverStruct.end())
        {
            UId = serverStruct.find("UId")->second.GetString().c_str();
            Name = serverStruct.find("Name")->second.GetString().c_str();
            FileName = serverStruct.find("FileName")->second.GetString();
            Environment = serverStruct.find("Environnement")->second.GetString();
            Author = serverStruct.find("Author")->second.GetString();

            GoldTime = atoi(serverStruct.find("GoldTime")->second.GetString().c_str());
            CopperPrice = atoi(serverStruct.find("CopperPrice")->second.GetString().c_str());

            MapType = serverStruct.find("MapType")->second.GetString().c_str();
            MapStyle = serverStruct.find("MapStyle")->second.GetString().c_str();
        }
    }
};

#endif // MAP_H_
