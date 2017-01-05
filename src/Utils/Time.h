#ifndef TIME_H_
#define TIME_H_

#include <string>
#include <sstream>
#include <math.h>

//* Time
/**
 * \brief Contains utilities to format map times.
 */
class Time
{
public:
    /*!
     * \brief Formats time to 0:00.000 format.
     *
     * \param time         Time to be formatted.
     */
    static std::string FormatTime(int time)
    {
        std::string formattedTime = "0:00.000";

        if(time > 0)
        {
            std::stringstream timeAsStringstream;
            timeAsStringstream << time;
            std::string timeAsString = timeAsStringstream.str();

            int minutes = floor(time / (1000 * 60));
            int seconds = floor((time - (minutes * 60 * 1000)) / 1000);
            int tseconds = atoi(timeAsString.substr((timeAsString.length() - 3)).c_str());

            char formattedCharArray[8];
            sprintf(formattedCharArray, "%d:%02d.%03d", minutes, seconds, tseconds);

            return std::string(formattedCharArray);
        }

        return formattedTime;
    }

    /*!
     * \brief Returns the current time in yyyy-mm-dd hh:mm:ss format.
     */
    static std::string Current()
    {
        char buffer[20];
        time_t now = time(NULL);
        strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

        return std::string(buffer);
    }
};

#endif // TIME_H_
