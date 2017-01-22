#ifndef TEXT_H_
#define TEXT_H_

#include <regex>
#include <string>
#include <sstream>

//* Text
/**
 * \brief Contains utilities to format text.
 */
class Text
{
public:
    /*!
     * \brief Escapes XML signs, to send XML strings.
     *
     * \param src Source XML to be escaped.
     */
    static std::string EscapeXML(const std::string& src)
    {
        std::stringstream dst;
        for(char ch : src)
        {
            switch (ch)
            {
                case '&': dst << "&amp;"; break;
                case '\'': dst << "&apos;"; break;
                case '"': dst << "&quot;"; break;
                case '<': dst << "&lt;"; break;
                case '>': dst << "&gt;"; break;
                default: dst << ch; break;
            }
        }

        return dst.str();
    }
};

#endif // TEXT_H_
