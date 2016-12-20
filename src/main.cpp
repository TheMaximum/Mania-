#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "Config/Config.h"
#include "Config/Version.h"
#include "GbxRemote/GbxRemote.h"
#include "GbxRemote/GbxParameters.h"
#include "GbxRemote/GbxResponse.h"

int main(int argc, char *argv[])
{
    std::cout << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
    Config* config = new Config("config.yaml");
    std::cout << "Config 'config.yaml' loaded." << std::endl;

    GbxRemote server;
    if(server.InitWithIp(config->Server->address, config->Server->port))
    {
        std::cout << "Current protocol: GBXRemote " << server.GetProtocol() << std::endl;

        GbxParameters* params = new GbxParameters();
        params->Put(&config->Server->username);
        params->Put(&config->Server->password);
        server.Query(new GbxMessage("Authenticate", params));

        std::vector<GbxResponseParameter>* responseParams = server.GetResponse()->GetParameters();
        GbxResponseParameter responseParam = responseParams->at(0);
        std::cout << "Authenticate: " << responseParams->at(0).GetString() << " (" << responseParams->at(0).Type << ")" << std::endl;

        std::string message = "$fffMania++$ff0 is now online!";
        params = new GbxParameters();
        params->Put(&message);
        server.Query(new GbxMessage("ChatSendServerMessage", params));
        responseParams = server.GetResponse()->GetParameters();
        std::cout << "ChatSendServerMessage: " << responseParams->at(0).GetString() << " (" << responseParams->at(0).Type << ")" << std::endl;

        GbxMessage* getMethods = new GbxMessage("system.listMethods");
        if(server.Query(getMethods))
        {
            responseParams = server.GetResponse()->GetParameters();
            std::vector<GbxResponseParameter>* methodsArray = responseParams->at(0).GetArray();
            std::cout << "Amount of methods: " << methodsArray->size() << " (" << responseParams->at(0).Type << ")" << std::endl;
            for(int methodId = 0; methodId < methodsArray->size(); methodId++)
            {
                std::cout << "Method " << methodId << ": " << methodsArray->at(methodId).GetString() << " (" << methodsArray->at(methodId).Type << ")" << std::endl;
            }
        }
        else
        {
            GbxError* error = server.GetCurrentError();
            std::cout << "[ERROR " << error->number << "] " << error->message << std::endl;
        }
    }
    else
    {
        GbxError* error = server.GetCurrentError();
        std::cout << "[ERROR " << error->number << "] " << error->message << std::endl;
    }

    server.Terminate();
}
