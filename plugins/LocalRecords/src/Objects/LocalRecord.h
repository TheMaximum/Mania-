#ifndef LOCALRECORD_H_
#define LOCALRECORD_H_

#include <cppconn/resultset.h>

#include "Utils/Time.h"

class LocalRecord
{
public:
    int Id = 0;
    Map RecordMap;
    std::string Login;
    std::string NickName;

    int Time;
    std::string FormattedTime;
    std::vector<int> Checkpoints;

    std::string Date;

    LocalRecord()
    {
        
    }

    LocalRecord(sql::ResultSet* result)
    {
        Id = result->getInt("Id");
        Time = result->getInt("Score");
        FormattedTime = Time::FormatTime(Time);
        std::stringstream checkpoints(result->getString("Checkpoints"));
        while(checkpoints.good())
        {
            std::string checkpoint;
            getline(checkpoints, checkpoint, ',');
            Checkpoints.push_back(atoi(checkpoint.c_str()));
        }

        Date = result->getString("Date");
    }
};

#endif // LOCALRECORD_H_
