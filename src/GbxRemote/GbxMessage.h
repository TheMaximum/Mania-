#ifndef GBXMESSAGE_H_
#define GBXMESSAGE_H_

#include <string>
#include <vector>

#include "GbxParameters.h"

//* GbxMessage
/**
 * \brief XML-fies the message for communication with the server.
 */
class GbxMessage
{
public:
    /*!
     * \brief Builds XML message.
     *
     * \param method Server method name.
     * \param params Params which belong to the method.
     */
    GbxMessage(std::string method, GbxParameters* parameters = NULL);

    /*!
     * \brief Returns the method name.
     */
    std::string GetMethod();

    /*!
     * \brief Returns the created XML message.
     */
    std::string GetXml();

private:
    std::string method; /**< \brief Server method name. */
    std::string xml;    /**< \brief Created XML message. */
};

#endif // GBXMESSAGE_H_
