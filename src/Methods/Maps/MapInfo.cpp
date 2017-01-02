#include "../Methods.h"

Map Methods::GetCurrentMapInfo()
{
    Map currentMap = Map();

    GbxMessage* message = new GbxMessage("GetCurrentMapInfo");
    if(server->Query(message))
    {
        delete message; message = NULL;

        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> mapStruct = responseParams.at(0).GetStruct();

        currentMap.MapDetailed(mapStruct);
    }

    return currentMap;
}

Map Methods::GetMapInfo(std::string fileName)
{
    Map mapInfo = Map();

    GbxParameters* params = new GbxParameters();
    params->Put(&fileName);

    GbxMessage* message = new GbxMessage("GetMapInfo", params);
    if(server->Query(message))
    {
        delete message; message = NULL;
        delete params; params = NULL;

        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> mapStruct = responseParams.at(0).GetStruct();

        mapInfo.MapDetailed(mapStruct);
    }

    return mapInfo;
}
