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
        struct hostent *he;
        struct in_addr **addr_list;

        // Resolve the hostname, its not an ip address
        if((he = gethostbyname(address.c_str())) == NULL)
        {
            // gethostbyname failed
            herror("gethostbyname");
            return false;
        }

        // Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **)he->h_addr_list;

        for(int i = 0; addr_list[i] != NULL; i++)
        {
            server.sin_addr = *addr_list[i];
            break;
        }
    }
    else
    {
        // Plain ip address
        server.sin_addr.s_addr = inet_addr(address.c_str());
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //perror("connect failed. Error");
        return false;
    }

    return true;
}

bool TcpClient::Send(std::string data)
{
    RequestHandle++;

    const char* messageData = data.c_str();
    unsigned long dataLength = (strlen(messageData));
    unsigned long messageLength = (dataLength + 8);
    char message[messageLength];
    memcpy(&message[0], &dataLength, 4);
    memcpy(&message[4], &RequestHandle, 4);
    memcpy(&message[8], messageData, dataLength);
    //message[7] = 0x80;

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

char* TcpClient::Receive(int size = 512)
{
    char buffer[size];
    char* reply = new char[(size+1)];
    int bytes_received = 0;

    while(bytes_received < size)
    {
        int response = recv(sock, &buffer[bytes_received], sizeof(buffer) - bytes_received, 0);
        if(response < 0)
        {
            std::cout << "recv failed " << response << std::endl;
        }

        bytes_received += response;
    }

    memcpy(reply, buffer, sizeof(buffer));
    reply[size] = '\0';
    return reply;
}

bool TcpClient::SearchForCallBacks(int timeout)
{
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
