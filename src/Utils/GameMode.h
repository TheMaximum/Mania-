#ifndef GAMEMODE_H_
#define GAMEMODE_H_

//* GameMode
/**
 * \brief Enum with all available game modes.
 */
enum GameMode
{
    Script = 0,
    Rounds = 1,
    TimeAttack = 2,
    Team = 3,
    Laps = 4,
    Cup = 5,
    Stunts = 6
};


//* GameModeConverter
/**
 * \brief Converts enum values into usable strings.
 */
class GameModeConverter
{
public:
    /*!
     * \brief Converts enum value into usable string.
     *
     * \param mode Mode to convert.
     */
    static std::string GetName(GameMode mode)
    {
        switch(mode)
        {
            case GameMode::Rounds:
                return "Rounds";
            case GameMode::TimeAttack:
                return "TimeAttack";
            case GameMode::Team:
                return "Team";
            case GameMode::Laps:
                return "Laps";
            case GameMode::Cup:
                return "Cup";
            case GameMode::Stunts:
                return "Stunts";

            case GameMode::Script:
            default:
                return "Script";
        }
    }

    /*!
     * \brief Converts enum value into usable Dedimania-accepted string (so only TA or Rounds - n/a if not Dedi-accepted).
     *
     * \param mode Mode to convert.
     */
    static std::string GetDediName(GameMode mode)
    {
        switch(mode)
        {
            case GameMode::TimeAttack:
                return "TA";
            case GameMode::Rounds:
            case GameMode::Team:
            case GameMode::Laps:
            case GameMode::Cup:
                return "Rounds";

            default:
                return "n/a";
        }
    }
};

#endif // GAMEMODE_H_
