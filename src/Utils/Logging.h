#ifndef LOGGING_H_
#define LOGGING_H_

#include <cstring>
#include <cmath>
#include "../GbxRemote/GbxStructs.h"
#include "../GbxRemote/ServerResponse/GbxServerResponse.h"

//* Logging
/**
 * \brief Utility to print information to the console.
 */
class Logging
{
public:
    /*!
     * \brief Prints error to console.
     *
     * \param error        Error structure.
     */
    static void PrintError(GbxError error)
    {
        std::stringstream numberString;
        numberString << error.number;
        const char* errorNumber = numberString.str().c_str();
        int errorNumberLength = strlen(errorNumber);
        int spaces = (9 - errorNumberLength);

        if(spaces > 1)
        {
            double halfSpaces = ((double)spaces / 2);

            std::stringstream output;
            output << "[\033[0;31m";
            for(int i = 0; i < ceil(halfSpaces); i++)
            {
                output << " ";
            }
            output << error.number;
            for(int i = 0; i < floor(halfSpaces); i++)
            {
                output << " ";
            }

            output << "\033[0;0m] ERROR: " + error.message;

            std::cout << output.str() << std::endl;
        }
        else
        {
            std::cout << "[\033[0;31m" << errorNumber << "\033[0;0m] ERROR: " << error.message << std::endl;
        }
    }

    /*!
     * \brief Prints error to console.
     *
     * \param number       Error number.
     * \param message      Error message.
     */
    static void PrintError(int number, std::string message)
    {
        GbxError error = GbxError();
        error.number = number;
        error.message = message;
        PrintError(error);
    }

    /*!
     * \brief Prints OK. in [         ]-spaces in console.
     */
    static void PrintOKFlush()
    {
        std::cout << "[   \033[0;32mOK.\033[0;0m" << std::endl;
    }

    /*!
     * \brief Prints Failed! in [         ]-spaces in console.
     */
    static void PrintFailedFlush()
    {
        std::cout << "[ \033[0;31mFAILED!\033[0;0m" << std::endl;
    }

    /*!
     * \brief Prints a GbxResponseParameter (for DEBUG purposes).
     *
     * \param parameter     Parameter to be printed.
     * \param paramId       Number of the parameter.
     * \param spaces        Spaces to be put before the parameter information.
     * \param parameterName Name of the parameter (struct value only).
     */
    static void PrintParameter(GbxResponseParameter parameter, int paramId, std::string spaces = "    ", std::string parameterName = "")
    {
        if(parameter.Type.find("array") != std::string::npos)
        {
            std::cout << spaces << "Parameter #" << paramId << ": array" << std::endl;
            if(parameterName != "")
            {
                std::cout << " (" << parameterName << ")";
            }
            spaces += "    ";
            std::vector<GbxResponseParameter> arrayParam = parameter.GetArray();
            for(int subParamId = 0; subParamId < arrayParam.size(); subParamId++)
            {
                GbxResponseParameter arrayParameter = arrayParam.at(subParamId);
                PrintParameter(arrayParameter, subParamId, spaces);
            }
        }
        else if(parameter.Type.find("struct") != std::string::npos)
        {
            std::cout << spaces << "Parameter #" << paramId << ": struct" << std::endl;
            if(parameterName != "")
            {
                std::cout << " (" << parameterName << ")";
            }
            spaces += "    ";
            std::map<std::string, GbxResponseParameter> structParam = parameter.GetStruct();
            int subParamId = 0;
            for(std::map<std::string, GbxResponseParameter>::iterator subParam = structParam.begin(); subParam != structParam.end(); ++subParam)
            {
                PrintParameter(subParam->second, subParamId, spaces, subParam->first);
                subParamId++;
            }
        }
        else
        {
            std::cout << spaces << "Parameter #" << paramId << ": " << parameter.GetString() << " (" << parameter.Type << ")";
            if(parameterName != "")
            {
                std::cout << " (" << parameterName << ")";
            }
            std::cout << std::endl;
        }
    }
};

#endif // LOGGING_H_
