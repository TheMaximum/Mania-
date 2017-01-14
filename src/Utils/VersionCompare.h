#ifndef VERSIONCOMPARE_H_
#define VERSIONCOMPARE_H_

#include <string>
#include <sstream>

//* VersionCompare
/**
 * \brief Compares string versions.
 */
class VersionCompare
{
public:
    /*!
     * Compares two string versions.
     *
     * \param current Current string version.
     * \param other   String version to compare.
     */
    static bool NewerThanCurrent(const std::string& current, const std::string& other)
    {
        int parsedCurrent[3], parsedOther[3];
        parseVersion(parsedCurrent, current);
        parseVersion(parsedOther, other);
        return std::lexicographical_compare(parsedCurrent, parsedCurrent + 3, parsedOther, parsedOther + 3);
    }

    /*!
     * Compares two string versions.
     *
     * \param base    Base string version.
     * \param comp    String version to compare.
     */
    static bool NotOlderThan(const std::string& base, const std::string& comp)
    {
        int parsedBase[3], parsedComp[3];
        parseVersion(parsedBase, base);
        parseVersion(parsedComp, comp);
        return !std::lexicographical_compare(parsedComp, parsedComp + 3, parsedBase, parsedBase + 3);
    }

private:
    /*!
     * Converts version string into comparable integer array.
     *
     * \param result  Integer array for the comparable result.
     * \param input   String input to be parsed.
     */
    static void parseVersion(int result[3], const std::string& input)
    {
        std::istringstream parser(input);
        parser >> result[0];
        for(int idx = 1; idx < 3; idx++)
        {
            parser.get();
            parser >> result[idx];
        }
    }
};

#endif // VERSIONCOMPARE_H_
