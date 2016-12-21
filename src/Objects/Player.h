#ifndef PLAYER_H_
#define PLAYER_H_

#include "../GbxRemote/GbxResponse.h"

struct Player
{
    int PlayerId;
    int TeamId;

    std::string Login;
    std::string NickName;

    bool IsSpectator;
    bool IsInOfficialMode;

    int LadderRanking;

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
