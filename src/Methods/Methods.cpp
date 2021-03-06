#include "Methods.h"

Methods::Methods(GbxRemote* serverPtr, std::map<std::string, Player>* playerListPtr)
{
    server = serverPtr;
    playerList = playerListPtr;
}

GbxError Methods::GetCurrentError()
{
    return server->GetCurrentError();
}
