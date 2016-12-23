#include "ManiaPP.h"

ManiaPP::ManiaPP()
{
    std::cout << "## Running Mania++ v" << VERSION << " ###########################################" << std::endl;

    config = new Config("config.yaml");
    logging = new Logging();
    server = new GbxRemote();
    players = new std::vector<Player>();
}

bool ManiaPP::ConnectToServer()
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

            params = new GbxParameters();
            std::string apiVersion = server->GetApiVersion();
            params->Put(&apiVersion);

            std::cout << "[         ] Setting API version to '" << apiVersion << "' ... " << '\r' << std::flush;
            if(server->Query(new GbxMessage("SetApiVersion", params)))
            {
                logging->PrintOKFlush();

                std::cout << "[         ] Retrieving server methods ... " << '\r' << std::flush;
                if(server->Query(new GbxMessage("system.listMethods")))
                {
                    responseParams = server->GetResponse()->GetParameters();
                    std::vector<GbxResponseParameter>* methodsArray = responseParams->at(0).GetArray();
                    std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieving server methods: " << methodsArray->size() << " found." << std::endl;

                    retrievePlayerList();

                    bool enableCallbacks = true;
                    GbxParameters* params = new GbxParameters();
                    params->Put(&enableCallbacks);
                    server->Query(new GbxMessage("EnableCallbacks", params));

                    return true;
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
    else
    {
        logging->PrintFailedFlush();
        logging->PrintError(server->GetCurrentError());
    }

    return false;
}

void ManiaPP::MainLoop()
{
    std::cout << "Starting our loop ..." << std::endl;
    while(true)
    {
        server->ReadCallBacks();
        std::vector<GbxCallBack*>* callBacks = server->GetCBResponses();
        if(callBacks->size() > 0)
        {
            for(int callBackId = 0; callBackId < callBacks->size(); callBackId++)
            {
                GbxCallBack* callBack = callBacks->at(callBackId);
                std::vector<GbxResponseParameter>* parameters = callBack->GetParameters();

                std::cout << "CALLBACK: " << callBack->GetMethodName() << " (parameters: " << parameters->size() << ")" << std::endl;
                for(int paramId = 0; paramId < parameters->size(); paramId++)
                {
                    GbxResponseParameter parameter = parameters->at(paramId);
                    PrintParameter(parameter, paramId);
                }
            }

            server->ResetCBResponses();
        }
    }
}

void ManiaPP::PrintParameter(GbxResponseParameter parameter, int paramId, std::string spaces)
{
    if(parameter.Type.find("array") != std::string::npos)
    {
        std::cout << spaces << "Parameter #" << paramId << ": array" << std::endl;
        spaces += "    ";
        std::vector<GbxResponseParameter>* arrayParam = parameter.GetArray();
        for(int subParamId = 0; subParamId < arrayParam->size(); subParamId++)
        {
            GbxResponseParameter arrayParameter = arrayParam->at(subParamId);
            PrintParameter(arrayParameter, subParamId, spaces);
        }
    }
    else if(parameter.Type.find("struct") != std::string::npos)
    {
        std::cout << spaces << "Parameter #" << paramId << ": struct" << std::endl;
        spaces += "    ";
        std::map<std::string, GbxResponseParameter>* structParam = parameter.GetStruct();
        int subParamId = 0;
        for(std::map<std::string, GbxResponseParameter>::iterator subParam = structParam->begin(); subParam != structParam->end(); ++subParam)
        {
            PrintParameter(subParam->second, subParamId, spaces);
            subParamId++;
        }
    }
    else
    {
        std::cout << spaces << "Parameter #" << paramId << ": " << parameter.GetString() << " (" << parameter.Type << ")" << std::endl;
    }
}

void ManiaPP::retrievePlayerList()
{
    std::cout << "[         ] Retrieving current player list ... " << '\r' << std::flush;
    int playerListLimit = 512; int playerListIndex = 0;
    GbxParameters* params = new GbxParameters();
    params->Put(&playerListLimit);
    params->Put(&playerListIndex);
    GbxMessage* getPlayerList = new GbxMessage("GetPlayerList", params);
    if(server->Query(getPlayerList))
    {
        std::vector<GbxResponseParameter>* responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter>* playerList = responseParams->at(0).GetArray();

        for(int playerId = 0; playerId < playerList->size(); playerId++)
        {
            std::map<std::string, GbxResponseParameter>* player = playerList->at(playerId).GetStruct();
            players->push_back(Player(player));
        }

        std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieving current player list: " << players->size() << " found." << std::endl;

        for(int playerInList = 0; playerInList < players->size(); playerInList++)
        {
            Player listPlayer = players->at(playerInList);
            std::cout << "Player #" << listPlayer.PlayerId << ":" << std::endl;
            std::cout << "    Team #           : " << listPlayer.TeamId << std::endl;
            std::cout << "    Login            : " << listPlayer.Login << std::endl;
            std::cout << "    NickName         : " << listPlayer.NickName << std::endl;
            std::cout << "    SpectatorStatus  : " << listPlayer.SpectatorStatus << std::endl;
            std::cout << "    Flags            : " << listPlayer.Flags << std::endl;
            std::cout << "    LadderRanking    : " << listPlayer.LadderRanking << std::endl;
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
