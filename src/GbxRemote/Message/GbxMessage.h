#ifndef GBXMESSAGE_H_
#define GBXMESSAGE_H_

#include <string>
#include <vector>

#include "../Parameters/GbxParameters.h"
#include "../Parameters/GbxParameter.h"

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
     * \param methodName Server method name.
     * \param parameters Params which belong to the method.
     */
    GbxMessage(std::string methodName, GbxParameters* parameters = NULL);

    /*!
     * \brief Builds XML message.
     *
     * \param methodName Server method name.
     * \param parameters Params which belong to the method.
     */
    GbxMessage(std::string methodName, GbxParameters parameters);

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
