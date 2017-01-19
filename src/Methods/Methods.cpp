#include "Methods.h"

Methods::Methods(GbxRemote* serverPtr, std::map<std::string, Player>* playerListPtr, ServerInfo* serverInfoPtr)
{
    server = serverPtr;
    playerList = playerListPtr;
    serverInfo = serverInfoPtr;
}

GbxError Methods::GetCurrentError()
{
    return server->GetCurrentError();
}
