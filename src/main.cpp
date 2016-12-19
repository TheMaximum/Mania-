#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "Config/Config.h"
#include "Config/Version.h"
#include "GbxRemote/GbxRemote.h"

int main(int argc, char *argv[])
{
    std::cout << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
    Config* config = new Config("config.yaml");

    GbxRemote server;
    if(server.InitWithIp(config->Server->address, config->Server->port))
    {
        std::cout << "Current protocol: GBXRemote " << server.GetProtocol() << std::endl;

        server.Query(new GbxMessage("Authenticate", new std::vector<void*> { &config->Server->username, &config->Server->password }));
        std::cout << "Authenticate: " << server.GetResponse() << std::endl;

        std::string message = "$fffMania++$ff0 is now online!";
        server.Query(new GbxMessage("ChatSendServerMessage", new std::vector<void*> { &message }));

        /*GbxMessage* getMethods = new GbxMessage("system.listMethods");
        if(server.Query(getMethods))
        {
            //server.GetResponse();
            //std::cout << "Methods: " << server.GetResponse() << std::endl;
        }
        else
        {
            GbxError* error = server.GetCurrentError();
            std::cout << "[ERROR " << error->number << "] " << error->message << std::endl;
        }*/
    }
    else
    {
        GbxError* error = server.GetCurrentError();
        std::cout << "[ERROR " << error->number << "] " << error->message << std::endl;
    }

    server.Terminate();
}
