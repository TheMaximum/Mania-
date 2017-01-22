#ifndef FORMATTING_H_
#define FORMATTING_H_

#include <regex>
#include <string>
#include <sstream>

#include <boost/algorithm/string/replace.hpp>

//* Formatting
/**
 * \brief Formats TM-input to strip colors/formatting.
 */
class Formatting
{
public:
    /*!
     * \brief Strips all TrackMania colors from a string.
     *
     * \param src   String to be de-colorfied.
     * \param forTM Output for TM chat? (default: true)
     */
    static std::string StripColors(std::string src, bool forTM = true)
    {
        std::regex color = std::regex("([$][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F])");
        src = std::regex_replace(src, color, "");

        std::regex wrongcolor = std::regex("([$][0-9a-fA-F][0-9a-fA-F])");
        src = std::regex_replace(src, wrongcolor, "");

        src = std::regex_replace(src, std::regex("([$][$])"), "\t");
        std::regex formats = std::regex("([$][^hlp])");
        src = std::regex_replace(src, formats, "");

        std::regex links = std::regex("[$][hlp](\\[.*?\\])");
        src = std::regex_replace(src, links, "");
        src = std::regex_replace(src, std::regex("([$][hlp])"), "");

        if(forTM)
        {
            boost::replace_all(src, "\t", "$$");
        }
        else
        {
            boost::replace_all(src, "\t", "$");
        }

        return src;
    }

    /*!
     * \brief Strips all TrackMania links from a string.
     *
     * \param src   String to be de-linkified.
     * \param forTM Output for TM chat? (default: true)
     */
    static std::string StripLinks(std::string src, bool forTM = true)
    {
        src = std::regex_replace(src, std::regex("([$][$])"), "\t");

        std::regex links = std::regex("[$][hlp](\\[.*?\\])");
        src = std::regex_replace(src, links, "");
        src = std::regex_replace(src, std::regex("([$][hlp])"), "");

        if(forTM)
        {
            boost::replace_all(src, "\t", "$$");
        }
        else
        {
            boost::replace_all(src, "\t", "$");
        }

        return src;
    }
};

#endif // FORMATTING_H_
