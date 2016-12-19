#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "Config/Version.h"
#include "GbxRemote/GbxRemote.h"

std::string host     = "***";
int         port     = 5000;
std::string login    = "SuperAdmin";
std::string password = "***";

int main(int argc, char *argv[])
{
    std::cout << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;

    GbxRemote server;
    if(server.InitWithIp(host, port))
    {
        std::cout << "Current protocol: GBXRemote " << server.GetProtocol() << std::endl;

        GbxMessage* authenticate = new GbxMessage("Authenticate", new std::vector<void*> { &login, &password });
        server.Query(authenticate);
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
