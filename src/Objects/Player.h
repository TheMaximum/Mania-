#ifndef PLAYER_H_
#define PLAYER_H_

#include "../GbxRemote/Response/GbxResponse.h"

//* Player
/**
 * \brief Contains all information about a player in easy-to-use format.
 */
struct Player
{
    // === Mania++ information ===
    int Id = 0;                          /**< \brief Player identifier (on database). */
    std::vector<int> CurrentCheckpoints; /**< \brief Checkpoint passes on current try. */

    // === Basic information ===
    int PlayerId;                        /**< \brief Player identifier (on server). */
    int TeamId;                          /**< \brief Team identifier (on server). */

    std::string Login;                   /**< \brief Player login. */
    std::string NickName;                /**< \brief Player nickname.*/

    int SpectatorStatus;                 /**< \brief Spectator status of player. */
    int Flags;                           /**< \brief Player flags. */

    int LadderRanking;                   /**< \brief Current ladder ranking of the player. */

    // === Detailed information ===
    std::string IPAddress;               /**< \brief IP Address of the player. */
    int DownloadRate;                    /**< \brief Download rate in Kbps. */
    int UploadRate;                      /**< \brief Upload rate in Kbps. */

    std::string Language;                /**< \brief Application language. */

    bool IsSpectator;                    /**< \brief Is in spectatormode? */
    bool IsInOfficialMode;               /**< \brief Is in official mode? */

    std::string ClientVersion;           /**< \brief Client version (f.e. 'v2015-06-18_15_10 (3.3.0)'). */
    std::string ClubLink;                /**< \brief Club URL (website, entered by player). */
    std::string Path;                    /**< \brief Player location without "world". */
    std::string Country;                 /**< \brief Path without continent. */

    /*!
     * \brief Constructs a Player object without input.
     */
    Player()
    {
        CurrentCheckpoints = std::vector<int>();
    }

    /*!
     * \brief Formats a server response into a usable form.
     *
     * \param serverStruct The struct with player information received from the server.
     */
    Player(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        CurrentCheckpoints = std::vector<int>();
        if(serverStruct.find("Login") != serverStruct.end())
        {
            setBasicInfo(serverStruct);
        }
    }

    /*!
     * \brief Formats a server response into a usable form.
     *
     * \param serverStruct The struct with player information received from the server.
     */
    void SetBasic(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        if(serverStruct.find("Login") != serverStruct.end())
        {
            setBasicInfo(serverStruct);
        }
    }

    /*!
     * \brief Formats a server response into a usable form.
     *
     * \param serverStruct The struct with detailed player information received from the server.
     */
    void PlayerDetailed(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        if(serverStruct.find("Language") != serverStruct.end())
        {
            setDetailedInfo(serverStruct);
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
     * \brief Sets the basic player information from the struct.
     *
     * \param serverStruct The struct with player information received from the server.
     */
    void setBasicInfo(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        PlayerId = atoi(serverStruct.find("PlayerId")->second.GetString().c_str());
        TeamId = atoi(serverStruct.find("TeamId")->second.GetString().c_str());

        Login = serverStruct.find("Login")->second.GetString();
        NickName = serverStruct.find("NickName")->second.GetString();

        SpectatorStatus = atoi(serverStruct.find("SpectatorStatus")->second.GetString().c_str());
        Flags = atoi(serverStruct.find("Flags")->second.GetString().c_str());

        LadderRanking = atoi(serverStruct.find("LadderRanking")->second.GetString().c_str());
    }

    /*!
     * \brief Sets the detailed player information from the struct.
     *
     * \param serverStruct The struct with detailed player information received from the server.
     */
    void setDetailedInfo(std::map<std::string, GbxResponseParameter> serverStruct)
    {
        IPAddress = serverStruct.find("IPAddress")->second.GetString();
        DownloadRate = atoi(serverStruct.find("DownloadRate")->second.GetString().c_str());
        UploadRate = atoi(serverStruct.find("UploadRate")->second.GetString().c_str());

        Language = serverStruct.find("Language")->second.GetString();

        std::istringstream(serverStruct.find("IsSpectator")->second.GetString()) >> IsSpectator;
        std::istringstream(serverStruct.find("IsInOfficialMode")->second.GetString()) >> IsInOfficialMode;

        ClientVersion = serverStruct.find("ClientVersion")->second.GetString();
        ClubLink = serverStruct.find("ClubLink")->second.GetString();

        std::string path = serverStruct.find("Path")->second.GetString().substr(6); // Remove World|
        Path = path; // Remove World|

        std::vector<std::string> continents = { "Europe|", "Africa|", "Asia|", "Middle East|", "North America|", "South America|", "Oceania|" };
        for(int continentId = 0; continentId < continents.size(); continentId++)
        {
            std::string continent = continents.at(continentId);
            std::string::size_type i = path.find(continent);
            if (i != std::string::npos)
               path.erase(i, continent.length());
        }

        Country = path;
    }
};

#endif // PLAYER_H_
