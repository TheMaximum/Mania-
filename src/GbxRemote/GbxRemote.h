#ifndef GBXREMOTE_H_
#define GBXREMOTE_H_

#include "../Socket/TcpClient.h"
#include <sstream>
#include <vector>

#include "GbxStructs.h"
#include "GbxMessage.h"
#include "GbxParameters.h"
#include "GbxResponse.h"

//* GbxRemote
/**
 * \brief Handles communication with the ManiaPlanet server.
 *
 * \todo Handle/de-XMLify callbacks (receiving works).
 * \todo Make it possible to set an API version without upsetting TinyXML2.
 */
class GbxRemote
{
public:
    /*!
     * Deletes and nullifies the currentError and currentResponse.
     */
    ~GbxRemote();

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
     * \brief Read callbacks from the server.
     *
     * Returns whether it found a callback.
     */
    bool ReadCallBacks();

    /*!
     * \brief Handles callbacks (de-XML-fies them).
     *
     * \param data     Raw response from the server.
     */
    void HandleCallBack(char* data);

    /*!
     * \brief Returns the response from the server.
     */
    GbxResponse* GetResponse();

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
    int protocol = 0;                               /**< \brief Protocol version (0 = uninitialized, 1 or 2 = version). */
    TcpClient server;                               /**< \brief Socket connection with the server. */

    GbxError* currentError = new GbxError;          /**< \brief Current server error. */
    GbxResponse* currentResponse = new GbxResponse; /**< \brief Current server response. */
};

#endif // GBXREMOTE_H_
