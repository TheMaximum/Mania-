#include <fstream>
#include "base64.h"

#include "../Methods.h"

std::string Methods::GetValidationReplay(std::string login)
{
    std::string response;

    GbxParameters params = GbxParameters();
    params.Put(&login);

    if(server->Query(GbxMessage("GetValidationReplay", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = responseParams.at(0).GetString();
    }

    return response;
}

std::string Methods::GetGhostReplay(std::string login, std::string fileName)
{
    std::string response;
    fileName = "GReplays/" + fileName;
    std::string gameDataDir = GameDataDirectory();
    std::string fullPath = (gameDataDir + "Replays/" + fileName);

    GbxParameters params = GbxParameters();
    params.Put(&login);
    params.Put(&fileName);

    if(server->Query(GbxMessage("SaveBestGhostsReplay", params)))
    {
        std::cout << "Getting content of '" << fullPath << "'" << std::endl;
        std::ifstream replayFile(fullPath);
        std::string fileContent((std::istreambuf_iterator<char>(replayFile)), (std::istreambuf_iterator<char>()));
        response = base64_encode(reinterpret_cast<const unsigned char*>(fileContent.c_str()), fileContent.length());
    }

    return response;
}
