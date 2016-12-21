#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "Config/Config.h"
#include "Config/Version.h"
#include "GbxRemote/GbxRemote.h"
#include "GbxRemote/GbxParameters.h"
#include "GbxRemote/GbxResponse.h"
#include "Utils/Logging.h"

int main(int argc, char *argv[])
{
    std::cout << "## Running Mania++ v" << VERSION << " ########################################" << std::endl;
    Config* config = new Config("config.yaml");
    Logging logging = Logging();

    std::cout << "[         ] Connecting with server on " << config->Server->address << ":" << config->Server->port << " ... " << '\r' << std::flush;
    GbxRemote server;
    if(server.InitWithIp(config->Server->address, config->Server->port))
    {
        std::cout << "[   \033[0;32mOK.\033[0;0m" << std::endl;
        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Current protocol: GBXRemote " << server.GetProtocol() << std::endl;

        std::cout << "[         ] Authenticating on the server with user '" << config->Server->username << "' ... " << '\r' << std::flush;
        GbxParameters* params = new GbxParameters();
        params->Put(&config->Server->username);
        params->Put(&config->Server->password);

        if(server.Query(new GbxMessage("Authenticate", params)))
        {
            GbxResponse* response = server.GetResponse();
            std::vector<GbxResponseParameter>* responseParams = response->GetParameters();

            std::cout << "[   \033[0;32mOK.\033[0;0m" << std::endl;

            std::cout << "[         ] Retrieving server methods ... " << '\r' << std::flush;
            GbxMessage* getMethods = new GbxMessage("system.listMethods");
            if(server.Query(getMethods))
            {
                responseParams = server.GetResponse()->GetParameters();
                std::vector<GbxResponseParameter>* methodsArray = responseParams->at(0).GetArray();
                std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieving server methods: " << methodsArray->size() << " found." << std::endl;
                /*for(int methodId = 0; methodId < methodsArray->size(); methodId++)
                {
                    //std::cout << "Method " << methodId << ": " << methodsArray->at(methodId).GetString() << " (" << methodsArray->at(methodId).Type << ")" << std::endl;
                }*/
            }
            else
            {
                std::cout << "[ \033[0;31mFAILED!\033[0;0m" << std::endl;
                GbxError* error = server.GetCurrentError();
                std::cout << "[ERROR " << error->number << "] " << error->message << std::endl;
            }

            std::string message = "$fffMania++$ff0 is now online!";
            params = new GbxParameters();
            params->Put(&message);
            server.Query(new GbxMessage("ChatSendServerMessage", params));
            responseParams = server.GetResponse()->GetParameters();
            std::cout << "ChatSendServerMessage: " << responseParams->at(0).GetString() << " (" << responseParams->at(0).Type << ")" << std::endl;
        }
        else
        {
            std::cout << "[ \033[0;31mFAILED!\033[0;0m" << std::endl;
            logging.PrintError(server.GetCurrentError());
        }
    }
    else
    {
        std::cout << "[ \033[0;31mFAILED!\033[0;0m" << std::endl;
        logging.PrintError(server.GetCurrentError());
    }

    server.Terminate();
}
