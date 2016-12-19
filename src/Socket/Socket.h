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

struct send_message
{
    unsigned long strlen;
    unsigned long reqhandle;
    const char* request;
};

class tcp_client
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    tcp_client();
    void close();
    
    bool conn(std::string, int);
    bool send_data(std::string);
    char* receive(int);

    int reqhandle = 0x80000000;
};

#endif // SOCKET_H_
