#ifndef EVENTS_STRUCTS_H_
#define EVENTS_STRUCTS_H_

#include "../Objects/Player.h"

//* EntryVal
/**
 * \brief Struct with entry information.
 */
struct EntryVal
{
    std::string Name;  /**< \brief Entry name. */
    std::string Value; /**< \brief Entry value. */
};

struct PlayerRanking
{
    Player PlayerInfo;
    int Rank;
    int BestTime;
    std::vector<int> BestCheckpoints;
    int Score;
    int NbrLapsFinished;
    double LadderScore;
};

#endif // EVENTS_STRUCTS_H_
