#ifndef PLAYER_H_
#define PLAYER_H_

#include "../GbxRemote/Response/GbxResponse.h"

//* Player
/**
 * \brief Contains all information about a player in easy-to-use format.
 */
struct Player
{
    int PlayerId;          /**< \brief Player identifier (on server). */
    int TeamId;            /**< \brief Team identifier (on server). */

    std::string Login;     /**< \brief Player login. */
    std::string NickName;  /**< \brief Player nickname.*/

    bool IsSpectator;      /**< \brief Is player a spectator? */
    bool IsInOfficialMode; /**< \brief Is player in official mode? */

    int LadderRanking;     /**< \brief Current ladder ranking of the player. */

    /*!
     * \brief Formats a server response into a usable form.
     *
     * \param serverStruct The struct with player information received from the server.
     */
    Player(std::map<std::string, GbxResponseParameter>* serverStruct)
    {
        if(serverStruct->find("Login") != serverStruct->end())
        {
            PlayerId = atoi(serverStruct->find("PlayerId")->second.GetString().c_str());
            TeamId = atoi(serverStruct->find("TeamId")->second.GetString().c_str());

            Login = serverStruct->find("Login")->second.GetString();
            NickName = serverStruct->find("NickName")->second.GetString();

            IsSpectator = (bool)atoi(serverStruct->find("IsSpectator")->second.GetString().c_str());
            IsInOfficialMode = (bool)atoi(serverStruct->find("IsInOfficialMode")->second.GetString().c_str());

            LadderRanking = atoi(serverStruct->find("LadderRanking")->second.GetString().c_str());
        }
    }
};

#endif // PLAYER_H_
