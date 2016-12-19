/**
    C++ client example using sockets
*/
#include "Socket.h"

using namespace std;

tcp_client::tcp_client()
{
    sock = -1;
    port = 0;
    address = "";
}

void tcp_client::close()
{
    shutdown(sock, 2);
}

/**
    Connect to a host on a certain port number
*/
bool tcp_client::conn(string address , int port)
{
    //create socket if it is not already created
    if(sock == -1)
    {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }

        cout<<"Socket created\n";
    }
    else    {   /* OK , nothing */  }

    //setup address structure
    if(inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout<<"Failed to resolve hostname\n";

            return false;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;

        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];

            cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;

            break;
        }
    }

    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    cout<<"Connected\n";
    return true;
}

bool tcp_client::send_data(string data)
{
    reqhandle++;

    const char* messageData = data.c_str();
    unsigned long dataLength = (strlen(messageData));
    unsigned long messageLength = (dataLength + 8);
    char message[messageLength];
    memcpy(&message[0], &dataLength, 4);
    memcpy(&message[4], &reqhandle, 4);
    memcpy(&message[8], messageData, dataLength);
    message[7] = 0x80;

    Hex::Print(message, messageLength);

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

/**
    Receive data from the connected host
*/
char* tcp_client::receive(int size=512)
{
    char buffer[size];
    char* reply = new char[(size+1)];
    int bytes_received = 0;

    //Receive a reply from the server
    while(bytes_received < size)
    {
        int response = recv(sock, &buffer[bytes_received], sizeof(buffer) - bytes_received, 0);
        if(response < 0)
        {
            std::cout << "recv failed " << response << std::endl;
        }

        bytes_received += response;
    }

    reply = buffer;
    reply[size] = '\0';
    return reply;
}

/*char* tcp_client::receive(int size=512)
{
    char buffer[size];
    char* reply = new char[(size+1)];
    //int bytes_received = 0;

    //Receive a reply from the server
    //while()
    int response = recv(sock, buffer, sizeof(buffer), 0);
    if(response < 0)
    {
        std::cout << "recv failed " << response << std::endl;
    }

    reply = buffer;
    reply[size] = '\0';
    return reply;
}*/

/*std::string tcp_client::receive_in_chunks(int size)
{
    int bytes_received = 0;
    std::string message = "";

    while(bytes_received < size)
    {

    }

    return "";
}*/
