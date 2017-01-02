#include "GbxRemote.h"

GbxRemote::~GbxRemote()
{
    Terminate();

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
        std::string data = server.Receive(4);
        const GbxFirstResponse* response = reinterpret_cast<const GbxFirstResponse*>(data.c_str());

        int size = (int)response->size;
        if(size > 64)
        {
            currentError->number = -32300;
            std::stringstream messageStream;
            messageStream << "transport error - wrong lowlevel protocol header (" << size << ")";
            currentError->message = messageStream.str();
            return false;
        }

        std::string protocolRes = server.Receive(size);
        if(protocolRes.find("GBXRemote 1") != std::string::npos)
        {
            currentError->number = -32300;
            currentError->message = "transport error - old version of trackmania server detected";
            return false;
        }
        else if(protocolRes.find("GBXRemote 2") != std::string::npos)
        {
            protocol = 2;
        }
        else
        {
            currentError->number = -32300;
            std::stringstream messageStream;
            messageStream << "transport error - wrong lowlevel protocol version (" << protocolRes << ")";
            currentError->message = messageStream.str();
            return false;
        }

        connected = true;
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

    connected = false;
}

bool GbxRemote::Query(GbxMessage query)
{
    delete currentError;
    delete currentResponse;

    if(!connected)
        return false;

    currentError = new GbxError();
    currentResponse = new GbxResponse();

    if(!server.Send(query.GetXml()))
    {
        currentError->number = -32300;
        currentError->message = "transport error - connection interrupted!";
        return false;
    }

    while(true)
    {
        std::string data = server.Receive(8);
        const GbxQueryResponse* message = reinterpret_cast<const GbxQueryResponse*>(data.c_str());

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
            std::string rawResponse = server.Receive(message->size);

            if(rawResponse.find("methodCall") != std::string::npos)
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
    if(!connected)
        return false;

    if(server.SearchForCallBacks(2000))
    {
        std::string data = server.Receive(8);
        const GbxQueryResponse* message = reinterpret_cast<const GbxQueryResponse*>(data.c_str());
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
            std::string callback = server.Receive(size);
            if(callback.find("methodCall") != std::string::npos)
            {
                HandleCallBack(callback);
                return true;
            }
        }
    }

    return false;
}

void GbxRemote::HandleCallBack(std::string data)
{
    GbxCallBack callBack = GbxCallBack();
    callBack.SetRaw(data);
    currentCallBacks.push_back(callBack);
}

std::vector<GbxCallBack> GbxRemote::GetCBResponses()
{
    return currentCallBacks;
}

void GbxRemote::ResetCBResponses()
{
    currentCallBacks = std::vector<GbxCallBack>();
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
        return currentResponse;
    }

    return NULL;
}

int GbxRemote::GetProtocol()
{
    return protocol;
}

std::string GbxRemote::GetApiVersion()
{
    return apiVersion;
}
