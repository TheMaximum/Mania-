#ifndef GBXPARAMETER_H_
#define GBXPARAMETER_H_

#include <string>
#include <iostream>
#include <cxxabi.h>

#include "GbxParameters.h"

//* GbxParameter
/**
 * \brief XML-fies the parameter for communication with the server.
 */
class GbxParameter
{
public:
    /*!
     * \brief Casts parameter to string and calls calculateType.
     *
     * \param param Parameter to be xml-fied.
     */
    GbxParameter(GbxParam param);

    /*!
     * \brief Returns xml-fied parameter.
     */
    std::string GetXml();

private:
    std::string data;    /**< \brief Stringified parameter. */
    std::string type;    /**< \brief Stringified parameter type for XML. */
    std::string xmlType; /**< \brief Stringified parameter type for XML. */

    /*!
     * \brief Determines which XML-RPC type the parameter has.
     *
     * \param param Type information about the parameter.
     */
    void determineType(const std::type_info* param);

    /*!
     * \brief Converts the void pointer into the type determined in determineType.
     *
     * \param param Void pointer to parameter.
     */
    void dereferenceData(void* param);
};

#endif // GBXPARAMETER_H_
