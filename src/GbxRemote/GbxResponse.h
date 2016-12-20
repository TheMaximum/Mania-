#ifndef GBXRESPONSE_H_
#define GBXRESPONSE_H_

#include <string>
#include <vector>
#include <iostream>
#include "tinyxml2.h"

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
     * \brief Gets the value as string.
     */
    std::vector<GbxResponseParameter>* GetArray()
    {
        if(Type.find("array") != std::string::npos)
        {
            return static_cast<std::vector<GbxResponseParameter>*>(Value);
        }

        return NULL;
    }

    /*!
     * \brief Gets the value as string.
     */
    std::string GetString()
    {
        if(Type.find("array") == std::string::npos)
        {
            std::string* valuePtr = static_cast<std::string*>(Value);
            //std::string value = *valuePtr;
            return valuePtr->c_str();
        }

        return NULL;
    }

    std::string* GetStringPtr()
    {
        if(Type.find("array") == std::string::npos)
        {
            return static_cast<std::string*>(Value);
            //std::string value = *valuePtr;
            //return valuePtr->c_str();
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
    std::vector<GbxResponseParameter> GetParameters();

private:
    char* data;    /**< \brief Raw response data. */

    std::vector<GbxResponseParameter> parameters;

    /*!
     * \brief Extracts parameters from the raw data (XML).
     *
     * \todo Add support for structs.
     */
    void extractParameters();

    /*!
     * \brief Extracts parameters from the raw data (XML).
     *
     * \todo Make passing the parameter work everywhere.
     */
    GbxResponseParameter* extractParam(tinyxml2::XMLElement* param);
};

#endif // GBXRESPONSE_H_
