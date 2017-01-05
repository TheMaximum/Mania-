#ifndef LOCALRECORDSLIST_H_
#define LOCALRECORDSLIST_H_

class LocalRecordsList
{
public:
    std::vector<LocalRecord> List;

    int GetPlayerRecordIndex(Player player)
    {
        int playerRecord = 0;
        for(int recordId = 0; recordId < List.size(); recordId++)
        {
            LocalRecord localRecord = List.at(recordId);
            if(localRecord.Login == player.Login)
            {
                playerRecord = (recordId + 1);
                break;
            }
        }

        return playerRecord;
    }

    LocalRecord GetPlayerRecord(Player player)
    {
        LocalRecord playerRecord = LocalRecord();
        for(int recordId = 0; recordId < List.size(); recordId++)
        {
            LocalRecord localRecord = List.at(recordId);
            if(localRecord.Login == player.Login)
            {
                playerRecord = localRecord;
                break;
            }
        }

        return playerRecord;
    }
};

#endif // LOCALRECORDSLIST_H_
