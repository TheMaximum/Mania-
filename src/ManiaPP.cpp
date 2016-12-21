#include "ManiaPP.h"

ManiaPP::ManiaPP()
{
    std::cout << "## Running Mania++ v" << VERSION << " ###########################################" << std::endl;

    config = new Config("config.yaml");
    logging = new Logging();
    server = new GbxRemote();
}

void ManiaPP::ConnectToServer()
{
    std::cout << "[         ] Connecting with server on " << config->Server->address << ":" << config->Server->port << " ... " << '\r' << std::flush;
    if(server->InitWithIp(config->Server->address, config->Server->port))
    {
        logging->PrintOKFlush();
        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Current protocol: GBXRemote " << server->GetProtocol() << std::endl;

        std::cout << "[         ] Authenticating on the server with user '" << config->Server->username << "' ... " << '\r' << std::flush;
        GbxParameters* params = new GbxParameters();
        params->Put(&config->Server->username);
        params->Put(&config->Server->password);

        if(server->Query(new GbxMessage("Authenticate", params)))
        {
            GbxResponse* response = server->GetResponse();
            std::vector<GbxResponseParameter>* responseParams = response->GetParameters();

            logging->PrintOKFlush();

            std::cout << "[         ] Retrieving server methods ... " << '\r' << std::flush;
            GbxMessage* getMethods = new GbxMessage("system.listMethods");
            if(server->Query(getMethods))
            {
                responseParams = server->GetResponse()->GetParameters();
                std::vector<GbxResponseParameter>* methodsArray = responseParams->at(0).GetArray();
                std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieving server methods: " << methodsArray->size() << " found." << std::endl;
            }
            else
            {
                logging->PrintFailedFlush();
                logging->PrintError(server->GetCurrentError());
            }
        }
        else
        {
            logging->PrintFailedFlush();
            logging->PrintError(server->GetCurrentError());
        }
    }
    else
    {
        logging->PrintFailedFlush();
        logging->PrintError(server->GetCurrentError());
    }
}

void ManiaPP::Terminate()
{
    server->Terminate();
}
