#include "../Methods.h"

Map Methods::GetCurrentMapInfo()
{
    Map currentMap = Map();

    if(server->Query(GbxMessage("GetCurrentMapInfo")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> mapStruct = responseParams.at(0).GetStruct();

        currentMap.MapDetailed(mapStruct);
    }

    return currentMap;
}

Map Methods::GetNextMapInfo()
{
    Map nextMap = Map();

    if(server->Query(GbxMessage("GetNextMapInfo")))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> mapStruct = responseParams.at(0).GetStruct();

        nextMap.MapDetailed(mapStruct);
    }

    return nextMap;
}

Map Methods::GetMapInfo(std::string fileName)
{
    Map mapInfo = Map();

    GbxParameters params = GbxParameters();
    params.Put(&fileName);

    if(server->Query(GbxMessage("GetMapInfo", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> mapStruct = responseParams.at(0).GetStruct();

        mapInfo.MapDetailed(mapStruct);
    }

    return mapInfo;
}

bool Methods::CheckMapForCurrentServerParams(std::string fileName)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&fileName);

    if(server->Query(GbxMessage("CheckMapForCurrentServerParams", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

std::vector<Map> Methods::GetMapList(int limit, int index)
{
    std::vector<Map> list = std::vector<Map>();

    GbxParameters params = GbxParameters();
    params.Put(&limit);
    params.Put(&index);

    if(server->Query(GbxMessage("GetMapList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::vector<GbxResponseParameter> mapList = responseParams.at(0).GetArray();

        for(int mapId = 0; mapId < mapList.size(); mapId++)
        {
            std::map<std::string, GbxResponseParameter> map = mapList.at(mapId).GetStruct();
            Map newMap = Map(map);
            list.push_back(newMap);
        }
    }

    return list;
}
