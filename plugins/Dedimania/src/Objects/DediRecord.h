#ifndef DEDIRECORD_H_
#define DEDIRECORD_H_

#include "Utils/Time.h"

class DediRecord
{
public:
    int Rank = 0;
    std::string Login;
    std::string NickName;

    int Time;
    std::string FormattedTime;
    std::vector<int> Checkpoints;

    DediRecord()
    {

    }

    DediRecord(std::map<std::string, GbxResponseParameter> result)
    {
        Rank = atoi(result.find("Rank")->second.GetString().c_str());
        Login = result.find("Login")->second.GetString();
        NickName = result.find("NickName")->second.GetString();

        Time = atoi(result.find("Best")->second.GetString().c_str());
        FormattedTime = Time::FormatTime(Time);

        std::stringstream checkpoints(result.find("Checks")->second.GetString());
        while(checkpoints.good())
        {
            std::string checkpoint;
            getline(checkpoints, checkpoint, ',');
            Checkpoints.push_back(atoi(checkpoint.c_str()));
        }
    }

    std::string GetCheckpoints()
    {
        std::stringstream checkPoints;
        for(int cpId = 0; cpId < Checkpoints.size(); cpId++)
        {
            if(cpId > 0) checkPoints << ",";
            checkPoints << Checkpoints.at(cpId);
        }

        return checkPoints.str();
    }
};

#endif // DEDIRECORD_H_
