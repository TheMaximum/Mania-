#ifndef GBXSERVERRESPONSE_H_
#define GBXSERVERRESPONSE_H_

#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include "../../../lib/pugixml/src/pugixml.hpp"
#include "../GbxStructs.h"

//* GbxResponseParameter
/**
 * \brief Parameter deducted from server response.
 */
class GbxResponseParameter
{
public:
    std::string Type;  /**< \brief XML-RPC data type. */
    std::vector<GbxResponseParameter> Array;
    std::map<std::string, GbxResponseParameter> Struct;
    std::string Text;

    /*!
     * \brief Gets the value as vector of parameters.
     */
    std::vector<GbxResponseParameter> GetArray()
    {
        if(Type.find("array") != std::string::npos)
        {
            //std::vector<GbxResponseParameter>* vectorPtr = static_cast<std::vector<GbxResponseParameter>*>(Value);
            return Array;
        }

        return std::vector<GbxResponseParameter>();
    }

    /*!
     * \brief Gets the value as map of parameters.
     */
    std::map<std::string, GbxResponseParameter> GetStruct()
    {
        if(Type.find("struct") != std::string::npos)
        {
            //std::map<std::string, GbxResponseParameter>* mapPtr = static_cast<std::map<std::string, GbxResponseParameter>*>(Value);
            return Struct;
        }

        return std::map<std::string, GbxResponseParameter>();
    }

    /*!
     * \brief Gets the value as string.
     */
    std::string GetString()
    {
        if(Type.find("array") == std::string::npos &&
           Type.find("struct") == std::string::npos)
        {
            //std::string* valuePtr = static_cast<std::string*>(Value);
            //std::string value(valuePtr);
            return Text;
        }

        return NULL;
    }
};

//* GbxServerResponse
/**
 * \brief Response from server, de-XML-fies the response.
 */
class GbxServerResponse
{
public:
    /*!
     * \brief Empty constructor.
     */
    GbxServerResponse();

    /*!
     * Delets and nullifies the parameters.
     */
    ~GbxServerResponse();

    /*!
     * \brief Sets the raw message value.
     *
     * \param response Raw response from server (XML).
     */
    void SetRaw(std::string response);

    /*!
     * \brief Returns raw response (XML).
     */
    std::string GetRaw();

    /*!
     * \brief Returns the extracted parameters.
     */
    std::vector<GbxResponseParameter> GetParameters();

protected:
    std::string data; /**< \brief Raw response data. */
    std::vector<GbxResponseParameter> parameters = std::vector<GbxResponseParameter>(); /**< \brief List of parameters. */

    /*!
     * \brief Extracts parameters from the raw data (XML).
     */
    GbxResponseParameter extractParam(pugi::xml_node param);
};

#endif // GBXSERVERRESPONSE_H_
