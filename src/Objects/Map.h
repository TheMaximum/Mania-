#ifndef MAP_H_
#define MAP_H_

#include "../GbxRemote/Response/GbxResponse.h"

//* Map
/**
 * \brief Contains all information about a map in easy-to-use format.
 */
struct Map
{
    // === Mania++ information ===
    int Id = 0;               /**< \brief Map identifier (on database). */

    // === Basic information ===
    std::string UId;          /**< \brief Unique map identifier. */
    std::string Name;         /**< \brief Map name. */
    std::string FileName;     /**< \brief File name. */
    std::string Environment;  /**< \brief Game environment.*/
    std::string Author;       /**< \brief Login of the author.*/

    int GoldTime;             /**< \brief Time of the gold medal. */
    int CopperPrice;          /**< \brief Copper price. */

    std::string MapType;      /**< \brief Type of the map. */
    std::string MapStyle;     /**< \brief Map style (self-entered). */

    // === Detailed information ===
    std::string Mood;         /**< \brief Map mood (setting). */
    int BronzeTime;           /**< \brief Time of the bronze medal. */
    int SilverTime;           /**< \brief Time of the silver medal. */
    int AuthorTime;           /**< \brief Time set by the author. */

    bool LapRace;             /**< \brief Map is multi-lap. */
    int NbLaps;               /**< \brief Number of laps. */
    int NbCheckpoints;        /**< \brief Number of checkpoints. */

    /*!
     * \brief Constructs a Map object without input.
     */
    Map()
    {

    }

    /*!
     * \brief Formats a server response into a usable form.
     *
     * \param serverStruct The struct with map information received from the server.
     */
    Map(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        if(serverStruct.find("UId") != serverStruct.end())
        {
            setBasicInfo(serverStruct);
        }
    }

    /*!
     * \brief Formats a server response into a usable form.
     *
     * \param serverStruct The struct with detailed map information received from the server.
     */
    void MapDetailed(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        if(serverStruct.find("Mood") != serverStruct.end())
        {
            setDetailedInfo(serverStruct);
        }
    }

    /*!
     * \brief Copies information from map object.
     *
     * \param map          Map object to be copied into this.
     */
    void CopyDetailedMap(Map map)
    {
        if(!map.Mood.empty())
        {
            Mood = map.Mood;
            BronzeTime = map.BronzeTime;
            SilverTime = map.SilverTime;
            AuthorTime = map.AuthorTime;

            LapRace = map.LapRace;
            NbLaps = map.NbLaps;
            NbCheckpoints = map.NbCheckpoints;
        }
    }

    /*!
     * \brief Set database identifier value.
     *
     * \param id           Database ID.
     */
    void SetId(int id)
    {
        Id = id;
    }

private:
    /*!
     * \brief Sets the basic map information from the struct.
     *
     * \param serverStruct The struct with map information received from the server.
     */
    void setBasicInfo(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        UId = serverStruct.find("UId")->second.GetString();
        Name = serverStruct.find("Name")->second.GetString();
        FileName = serverStruct.find("FileName")->second.GetString();
        Environment = serverStruct.find("Environnement")->second.GetString();
        Author = serverStruct.find("Author")->second.GetString();

        GoldTime = atoi(serverStruct.find("GoldTime")->second.GetString().c_str());
        CopperPrice = atoi(serverStruct.find("CopperPrice")->second.GetString().c_str());

        MapType = serverStruct.find("MapType")->second.GetString();
        MapStyle = serverStruct.find("MapStyle")->second.GetString();
    }

    /*!
     * \brief Sets the detailed map information from the struct.
     *
     * \param serverStruct The struct with detailed map information received from the server.
     */
    void setDetailedInfo(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        UId = serverStruct.find("UId")->second.GetString();
        Name = serverStruct.find("Name")->second.GetString();
        FileName = serverStruct.find("FileName")->second.GetString();
        Environment = serverStruct.find("Environnement")->second.GetString();
        Author = serverStruct.find("Author")->second.GetString();

        GoldTime = atoi(serverStruct.find("GoldTime")->second.GetString().c_str());
        CopperPrice = atoi(serverStruct.find("CopperPrice")->second.GetString().c_str());

        MapType = serverStruct.find("MapType")->second.GetString();
        MapStyle = serverStruct.find("MapStyle")->second.GetString();

        std::string Mood = serverStruct.find("Mood")->second.GetString();
        int BronzeTime = atoi(serverStruct.find("BronzeTime")->second.GetString().c_str());
        int SilverTime = atoi(serverStruct.find("SilverTime")->second.GetString().c_str());
        int AuthorTime = atoi(serverStruct.find("AuthorTime")->second.GetString().c_str());

        std::istringstream(serverStruct.find("LapRace")->second.GetString()) >> LapRace;
        int NbLaps = atoi(serverStruct.find("NbLaps")->second.GetString().c_str());
        int NbCheckpoints = atoi(serverStruct.find("NbCheckpoints")->second.GetString().c_str());
    }
};

#endif // MAP_H_
