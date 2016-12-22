#include "GbxRemote.h"

GbxRemote::~GbxRemote()
{
    delete currentError;
    currentError = NULL;

    delete currentResponse;
    currentResponse = NULL;
}

bool GbxRemote::Init(int port)
{
    return InitWithIp("localhost", port);
}

bool GbxRemote::InitWithIp(std::string address, int port)
{
    apiVersion = (char*)"2015-02-10";
    if(server.Connect(address, port))
    {
        char* data = server.Receive(4);
        const GbxFirstResponse* response = reinterpret_cast<const GbxFirstResponse*>(data);
        int size = (int)response->size;
        if(size > 64)
        {
            currentError->number = -32300;
            std::stringstream messageStream;
            messageStream << "transport error - wrong lowlevel protocol header (" << size << ")";
            currentError->message = messageStream.str();
            return false;
        }

        char* protocolRes = server.Receive(size);
        char protocolResponse[(size+1)];
        strcpy(protocolResponse, protocolRes);

        if(strcmp(protocolResponse, "GBXRemote 1") == 0)
        {
            currentError->number = -32300;
            currentError->message = "transport error - old version of trackmania server detected";
            return false;
        }
        else if(strcmp(protocolResponse, "GBXRemote 2") == 0)
        {
            protocol = 2;
        }
        else
        {
            currentError->number = -32300;
            std::stringstream messageStream;
            messageStream << "transport error - wrong lowlevel protocol version (" << protocolResponse << ")";
            currentError->message = messageStream.str();
            return false;
        }

        return true;
    }
    else
    {
        currentError->number = -10;
        currentError->message = "connection error - could not establish connection";
        return false;
    }
}

void GbxRemote::Terminate()
{
    server.Close();
}

bool GbxRemote::Query(GbxMessage* query)
{
    currentError = new GbxError();
    currentResponse = new GbxResponse();

    if(!server.Send(query->GetXml()))
    {
        currentError->number = -32300;
        currentError->message = "transport error - connection interrupted!";
        return false;
    }

    while(true)
    {
        char* data = server.Receive(8);
        const GbxQueryResponse* message = reinterpret_cast<const GbxQueryResponse*>(data);

        if(message->size > (4096*1024))
        {
            currentError->number = -32300;
            std::stringstream errorMessage;
            errorMessage << "transport error - response too large " << message->size;
            currentError->message = errorMessage.str();
            return false;
        }

        if(message->size > 0)
        {
            char* rawResponse = server.Receive(message->size);

            if(std::string(rawResponse).find("methodCall") != std::string::npos)
            {
                HandleCallBack(rawResponse);
                continue;
            }

            currentResponse->SetRaw(rawResponse);
            if(currentResponse->GetFault() != NULL)
            {
                currentError = currentResponse->GetFault();
                return false;
            }

            return true;
        }
        else
        {
            return false;
        }
    }
}

bool GbxRemote::ReadCallBacks()
{
    if(server.SearchForCallBacks(2000))
    {
        char* data = server.Receive(8);
        const GbxQueryResponse* message = reinterpret_cast<const GbxQueryResponse*>(data);
        int size = message->size;

        if(size > (4096*1024))
        {
            currentError->number = -32300;
            std::stringstream errorMessage;
            errorMessage << "transport error - response too large " << size;
            currentError->message = errorMessage.str();
            return false;
        }

        if(size > 0)
        {
            char* callback = server.Receive(size);
            if(std::string(callback).find("methodCall") != std::string::npos)
            {
                HandleCallBack(callback);
                return true;
            }
        }
    }

    return false;
}

void GbxRemote::HandleCallBack(char* data)
{
    GbxCallBack* callBack = new GbxCallBack();
    callBack->SetRaw(data);
    currentCallBacks->push_back(callBack);
}

std::vector<GbxCallBack*>* GbxRemote::GetCBResponses()
{
    return currentCallBacks;
}

void GbxRemote::ResetCBResponses()
{
    currentCallBacks = new std::vector<GbxCallBack*>;
}

GbxError* GbxRemote::GetCurrentError()
{
    if(currentError->number == 0)
    {
        return NULL;
    }

    return currentError;
}

GbxResponse* GbxRemote::GetResponse()
{
    if(currentResponse->GetRaw() != "")
    {
        GbxResponse* returnResponse = currentResponse;
        currentResponse = new GbxResponse();
        return returnResponse;
    }

    return new GbxResponse();
}

int GbxRemote::GetProtocol()
{
    return protocol;
}

std::string GbxRemote::GetApiVersion()
{
    return apiVersion;
}
