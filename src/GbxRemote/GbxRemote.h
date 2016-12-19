#ifndef GBXREMOTE_H_
#define GBXREMOTE_H_

#include "../Socket/TcpClient.h"
#include <sstream>
#include <bitset>
#include <vector>

#include "GbxStructs.h"
#include "GbxMessage.h"

//* GbxRemote
/**
 * \brief Handles communication with the ManiaPlanet server.
 */
class GbxRemote
{
public:
    /*!
     * \brief Initializes connection with the local server.
     *
     * \param port     XML-RPC port of the server.
     */
    bool Init(int port);

    /*!
     * \brief Initializes connection with the server.
     *
     * \param address  Address of the server.
     * \param port     XML-RPC port of the server.
     */
    bool InitWithIp(std::string address, int port);

    /*!
     * \brief Closes the connection with the server.
     */
    void Terminate();

    /*!
     * \brief Sends a GbxMessage to the server.
     *
     * Returns whether the query was successfully sent.
     *
     * \param query    Query to be send.
     */
    bool Query(GbxMessage* query);

    /*!
     * \brief Returns the response from the server.
     *
     * \todo Decode received response (xml -> usable values).
     */
    char* GetResponse();

    /*!
     * \brief Returns the current server error.
     *
     * Returns NULL when there currently is no error.
     */
    GbxError* GetCurrentError();

    /*!
     * \brief Returns the current version number of the server protocol (1 or 2).
     */
    int GetProtocol();

private:
    int protocol = 0;                      /**< \brief Protocol version (0 = uninitialized, 1 or 2 = version). */
    TcpClient server;                      /**< \brief Socket connection with the server. */

    GbxError* currentError = new GbxError; /**< \brief Current server error. */
    char* currentResponse = NULL;          /**< \brief Current server response. */
};

#endif // GBXREMOTE_H_
