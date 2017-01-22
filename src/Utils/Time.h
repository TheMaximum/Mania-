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
            int tseconds = time;
            if(timeAsString.length() > 3)
            {
                tseconds = atoi(timeAsString.substr((timeAsString.length() - 3)).c_str());
            }

            char formattedCharArray[8];
            sprintf(formattedCharArray, "%d:%02d.%03d", minutes, seconds, tseconds);

            formattedTime = std::string(formattedCharArray);
        }

        return formattedTime;
    }

    /*!
     * \brief Formats time to 00:00:00 format.
     *
     * \param time         Time to be formatted.
     */
    static std::string FormatSeconds(int time)
    {
        std::string formattedTime = "00:00:00";

        if(time > 0)
        {
            int hours = floor(time / 60 / 60);
            int minutes = floor((time - (hours * 60 * 60)) / 60);
            int seconds = (time - (hours * 60 * 60) - (minutes * 60));

            if(hours > 999)
            {
                char formattedCharArray[10];
                sprintf(formattedCharArray, "%04d:%02d:%02d", hours, minutes, seconds);
                formattedTime = std::string(formattedCharArray);
            }
            else if(hours > 99)
            {
                char formattedCharArray[9];
                sprintf(formattedCharArray, "%03d:%02d:%02d", hours, minutes, seconds);
                formattedTime = std::string(formattedCharArray);
            }
            else
            {
                char formattedCharArray[8];
                sprintf(formattedCharArray, "%02d:%02d:%02d", hours, minutes, seconds);
                formattedTime = std::string(formattedCharArray);
            }
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
