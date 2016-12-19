#ifndef SOCKET_H_
#define SOCKET_H_

#include <iostream>    //cout
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <string>  //string
#include <sstream>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent

#include "../Utils/Hex.h"

//* TcpClient
/**
 * \brief Socket connection with server.
 *
 * Original version of class by Silver Moon.
 * Last updated on September 11th, 2012.
 * Retrieved from: http://www.binarytides.com/code-a-simple-socket-client-class-in-c/
 */
class TcpClient
{
public:
    /*!
     * \brief Initializes variables.
     */
    TcpClient();

    /*!
     * \brief Creates a socket connection with the server.
     *
     * Returns whether the connection was successfully established.
     *
     * \param address Address of the server (either hostname or IP address).
     * \param port    XML-RPC port of the server.
     */
    bool Connect(std::string address, int port);

    /*!
     * \brief Sends an XML message to the server.
     *
     * Returns whether the message was properly sent.
     *
     * \param data    XML message to send.
     */
    bool Send(std::string data);

    /*!
     * \brief Returns data received from the server.
     *
     * \param size    Size of the message expected.
     */
    char* Receive(int size);

    /*!
     * \brief Closes the socket with the server.
     */
    void Close();

    int RequestHandle = 0x80000000; /**< \brief Current request identifier. */

private:
    int sock;                       /**< \brief Socket connection with the server. */
    std::string address;            /**< \brief Address of the server. */
    int port;                       /**< \brief XML-RPC port of the server. */
    struct sockaddr_in server;      /**< \brief Server/socket information. */
};

#endif // SOCKET_H_
