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

//* PlayerRanking
/**
 * \brief Struct with player ranking.
 */
struct PlayerRanking
{
    Player PlayerInfo;   /**< \brief Player object. */
    int Rank;            /**< \brief Player rank. */
    int BestTime;        /**< \brief Best time this round. */
    std::vector<int> BestCheckpoints;  /**< \brief List of best checkpoints this round. */
    int Score;           /**< \brief Round score. */
    int NbrLapsFinished; /**< \brief Number of laps finished. */
    double LadderScore;  /**< \brief Ladder score received. */
};

#endif // EVENTS_STRUCTS_H_
