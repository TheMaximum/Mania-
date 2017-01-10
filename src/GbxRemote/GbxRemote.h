#ifndef GBXREMOTE_H_
#define GBXREMOTE_H_

#include "../Socket/TcpClient.h"
#include <sstream>
#include <vector>

#include "GbxStructs.h"
#include "Message/GbxMessage.h"
#include "Parameters/GbxParameters.h"
#include "Response/GbxResponse.h"
#include "CallBack/GbxCallBack.h"

//* GbxRemote
/**
 * \brief Handles communication with the ManiaPlanet server.
 *
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
    bool Query(GbxMessage query);

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
    void HandleCallBack(std::string data);

    /*!
     * \brief Returns the callbacks that have been received since last call.
     */
    std::vector<GbxCallBack> GetCBResponses();

    /*!
     * \brief Resets list of current callbacks.
     */
    void ResetCBResponses();

    /*!
     * \brief Returns the response from the server.
     */
    GbxResponse* GetResponse();

    /*!
     * \brief Returns the current server error.
     *
     * Returns empty GbxError when there currently is no error.
     */
    GbxError GetCurrentError();

    /*!
     * \brief Returns the current version number of the server protocol (1 or 2).
     */
    int GetProtocol();

    /*!
     * \brief Returns the set API version.
     */
    std::string GetApiVersion();

private:
    bool connected = false;                         /**< \brief Is the client connected with the server? */

    int protocol = 0;                               /**< \brief Protocol version (0 = uninitialized, 1 or 2 = version). */
    std::string apiVersion;                         /**< \brief Server API version. */
    TcpClient server;                               /**< \brief Socket connection with the server. */

    GbxError currentError = GbxError();          /**< \brief Current server error. */
    GbxResponse* currentResponse = new GbxResponse; /**< \brief Current server response. */
    std::vector<GbxCallBack> currentCallBacks = std::vector<GbxCallBack>(); /**< \brief List of currently received callbacks. */
};

#endif // GBXREMOTE_H_
