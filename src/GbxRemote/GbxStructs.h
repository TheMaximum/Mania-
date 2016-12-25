#ifndef GBXSTRUCTS_H_
#define GBXSTRUCTS_H_

//* GbxFirstResponse
/**
 * \brief Response received on handshake.
 */
struct GbxFirstResponse
{
    short size;  /**< \brief Size in bytes of the protocol message (usually 11). */
};

//* GbxQueryResponse
/**
 * \brief Response received after a query is sent.
 */
struct GbxQueryResponse
{
    short size;  /**< \brief Size in bytes of the query response. */
    int handle; /**< \brief Handle identifier of response message. */
};

//* GbxError
/**
 * \brief Stores error details from the communication with the server.
 */
struct GbxError
{
    int number = 0;       /**< \brief Number of the error (default: 0, no error). */
    std::string message;  /**< \brief Error message. */
};

#endif // GBXSTRUCTS_H_
