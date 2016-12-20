#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "Config/Config.h"
#include "Config/Version.h"
#include "GbxRemote/GbxRemote.h"
#include "GbxRemote/GbxParameters.h"

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
        std::cout << "Authenticate: " << server.GetResponse() << std::endl;

        std::string message = "$fffMania++$ff0 is now online!";
        params = new GbxParameters();
        params->Put(&message);
        server.Query(new GbxMessage("ChatSendServerMessage", params));
        std::cout << "ChatSendServerMessage: " << server.GetResponse() << std::endl;

        GbxMessage* getMethods = new GbxMessage("system.listMethods");
        if(server.Query(getMethods))
        {
            std::cout << "Methods: " << server.GetResponse() << std::endl;
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
