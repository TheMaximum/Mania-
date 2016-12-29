#include "TcpClient.h"

TcpClient::TcpClient()
{
    sock = -1;
    port = 0;
    address = "";

    RequestHandle = 0x80000000;
}

void TcpClient::Close()
{
    shutdown(sock, 2);
    close(sock);
    sock = -1;
}

bool TcpClient::Connect(std::string address, int port)
{
    // Create socket if it is not already created
    if(sock == -1)
    {
        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == -1)
        {
            perror("Could not create socket");
        }
    }

    // Setup address structure
    if(inet_addr(address.c_str()) == -1)
    {
        int sockfd;
        struct addrinfo hints, *servinfo, *p;
        struct sockaddr_in *h;
        int rv;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if((rv = getaddrinfo(address.c_str(), NULL, &hints, &servinfo)) != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            freeaddrinfo(servinfo);
            return false;
        }

        // loop through all the results and connect to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next)
        {
            h = (struct sockaddr_in *) p->ai_addr;
            address = inet_ntoa(h->sin_addr);
        }

        freeaddrinfo(servinfo); // all done with this structure
    }

    server.sin_addr.s_addr = inet_addr(address.c_str());

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        return false;
    }

    return true;
}

bool TcpClient::Send(std::string data)
{
    if(sock == -1)
        return false;

    RequestHandle++;

    const char* messageData = data.c_str();
    unsigned long dataLength = (strlen(messageData));
    unsigned long messageLength = (dataLength + 8);
    char message[messageLength];
    memcpy(&message[0], &dataLength, 4);
    memcpy(&message[4], &RequestHandle, 4);
    memcpy(&message[8], messageData, dataLength);

    int bytes_to_write = messageLength;
    while(bytes_to_write > 0)
    {
        int send_return = send(sock, message, bytes_to_write, 0);
        if(send_return < 0)
        {
            perror("Send failed : ");
            return false;
        }

        bytes_to_write -= send_return;
        if(bytes_to_write == 0)
            break;

        memcpy(message, (message + send_return), bytes_to_write);
    }

    return true;
}

std::string TcpClient::Receive(int size = 512)
{
    if(sock == -1)
        return NULL;

    std::string received;
    received.resize(size);
    int bytes_received = 0;

    while(bytes_received < size)
    {
        int response = read(sock, &received[bytes_received], (size - bytes_received));
        if(response < 0)
        {
            std::cout << "recv failed " << response << std::endl;
        }

        bytes_received += response;
    }

    received[bytes_received] = 0;
    return received;
}

bool TcpClient::SearchForCallBacks(int timeout)
{
    if(sock == -1)
        return false;

    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    tv.tv_sec = 0;
    tv.tv_usec = 2;
    int selectResult = select((sock + 1), &readfds, NULL, NULL, &tv);

    if(selectResult > 0)
    {
        return true;
    }

    return false;
}
