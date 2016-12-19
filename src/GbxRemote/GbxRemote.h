#ifndef GBXREMOTE_H_
#define GBXREMOTE_H_

#include "../Socket/Socket.h"
#include <sstream>
#include <bitset>
#include <vector>

struct first_response
{
    long size;
};

struct message_response
{
    short size;
    long handle;
};

struct GbxError
{
    int number;
    std::string message;
};

class GbxParameter
{
public:
    GbxParameter(void*);
    std::string GetXml();

private:
    std::string data;
    std::string type;
    std::string calculateType(void*);
};

class GbxMessage
{
public:
    GbxMessage(std::string method, std::vector<void*>* params);
    std::string GetXml();

private:
    std::string method;
    std::string xml;
};

class GbxRemote
{
public:
    bool Init(int port);
    bool InitWithIp(std::string host, int port);
    void Terminate();

    bool Query(GbxMessage* query);
    char* GetResponse();

    GbxError* GetCurrentError();
    int GetProtocol();

private:
    int protocol = 0;
    tcp_client server;

    GbxError* currentError = new GbxError;
    char* currentResponse = NULL;
};

#endif // GBXREMOTE_H_
