#ifndef GBXPARAMETER_H_
#define GBXPARAMETER_H_

#include <string>

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
    GbxParameter(void* param);
    
    /*!
     * \brief Returns xml-fied parameter.
     */
    std::string GetXml();

private:
    std::string data; /**< \brief Stringified parameter. */
    std::string type; /**< \brief Stringified parameter type. */

    /*!
     * \brief Determines which type the parameter has.
     *
     * \param param Parameter to be determined.
     */
    void calculateType(void* param);
};

#endif // GBXPARAMETER_H_
