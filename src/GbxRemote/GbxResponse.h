#ifndef GBXRESPONSE_H_
#define GBXRESPONSE_H_

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include "tinyxml2.h"

#include "GbxStructs.h"

//* GbxResponseParameter
/**
 * \brief Parameter deducted from server response.
 */
class GbxResponseParameter
{
public:
    std::string Type;  /**< \brief XML-RPC data type. */
    void* Value;       /**< \brief Data value (in text). */

    /*!
     * \brief Gets the value as vector of parameters.
     */
    std::vector<GbxResponseParameter>* GetArray()
    {
        if(Type.find("array") != std::string::npos)
        {
            std::vector<GbxResponseParameter>* vectorPtr = static_cast<std::vector<GbxResponseParameter>*>(Value);
            return vectorPtr;
        }

        return NULL;
    }

    /*!
     * \brief Gets the value as map of parameters.
     */
    std::map<std::string, GbxResponseParameter>* GetStruct()
    {
        if(Type.find("struct") != std::string::npos)
        {
            std::map<std::string, GbxResponseParameter>* mapPtr = static_cast<std::map<std::string, GbxResponseParameter>*>(Value);
            return mapPtr;
        }

        return NULL;
    }

    /*!
     * \brief Gets the value as string.
     */
    std::string GetString()
    {
        if(Type.find("array") == std::string::npos &&
           Type.find("struct") == std::string::npos)
        {
            char* valuePtr = static_cast<char*>(Value);
            std::string value(valuePtr);
            return value;
        }

        return NULL;
    }
};

//* GbxResponse
/**
 * \brief Response from server, de-XML-fies the response.
 */
class GbxResponse
{
public:
    /*!
     * \brief Empty constructor.
     */
    GbxResponse();

    /*!
     * Delets and nullifies the parameters.
     */
    ~GbxResponse();

    /*!
     * Returns whether the response is a fault.
     */
    GbxError* GetFault();

    /*!
     * \brief Sets the raw message value.
     *
     * \param response Raw response from server (XML).
     */
    void SetRaw(char* response);

    /*!
     * \brief Returns raw response (XML).
     */
    char* GetRaw();

    /*!
     * \brief Returns the extracted parameters.
     */
    std::vector<GbxResponseParameter>* GetParameters();

private:
    GbxError* fault = new GbxError; /**< \brief Current fault (error). */

    char* data;          /**< \brief Raw response data. */
    std::vector<GbxResponseParameter>* parameters = new std::vector<GbxResponseParameter>(); /**< \brief List of parameters. */

    /*!
     * \brief Extracts parameters from the raw data (XML).
     */
    void extractParameters();

    /*!
     * \brief Extracts parameters from the raw data (XML).
     */
    GbxResponseParameter extractParam(tinyxml2::XMLElement* param);
};

#endif // GBXRESPONSE_H_
