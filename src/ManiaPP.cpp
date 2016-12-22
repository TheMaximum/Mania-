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

            //params = new GbxParameters();
            //std::string apiVersion = server->GetApiVersion();
            //params->Put(&apiVersion);
            //server->Query(new GbxMessage("SetApiVersion", params));
            //GbxResponse* apiResponse = server->GetResponse();
            //std::cout << apiResponse->GetRaw() << std::endl;

            std::cout << "[         ] Retrieving server methods ... " << '\r' << std::flush;
            GbxMessage* getMethods = new GbxMessage("system.listMethods");
            if(server->Query(getMethods))
            {
                responseParams = server->GetResponse()->GetParameters();
                std::vector<GbxResponseParameter>* methodsArray = responseParams->at(0).GetArray();
                std::cout << "[   \033[0;32mOK.\033[0;0m   ] Retrieving server methods: " << methodsArray->size() << " found." << std::endl;

                bool enableCallbacks = true;
                GbxParameters* params = new GbxParameters();
                params->Put(&enableCallbacks);
                server->Query(new GbxMessage("EnableCallbacks", params));

                retrievePlayerList();

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
                std::cout << "CALLBACK: " << callBacks->at(callBackId)->GetMethodName() << " (parameters: " << callBacks->at(callBackId)->GetParameters()->size() << ")" << std::endl;
            }

            server->ResetCBResponses();
        }
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
            std::cout << "    IsSpectator      : " << listPlayer.IsSpectator << std::endl;
            std::cout << "    IsInOfficialMode : " << listPlayer.IsInOfficialMode << std::endl;
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
