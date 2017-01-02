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

Map Methods::GetNextMapInfo()
{
    Map nextMap = Map();

    GbxMessage* message = new GbxMessage("GetNextMapInfo");
    if(server->Query(message))
    {
        delete message; message = NULL;

        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::map<std::string, GbxResponseParameter> mapStruct = responseParams.at(0).GetStruct();

        nextMap.MapDetailed(mapStruct);
    }

    return nextMap;
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

bool Methods::CheckMapForCurrentServerParams(std::string fileName)
{
    bool response = false;

    GbxParameters* params = new GbxParameters();
    params->Put(&fileName);

    GbxMessage* message = new GbxMessage("CheckMapForCurrentServerParams", params);

    if(server->Query(message))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    delete params; params = NULL;
    delete message; message = NULL;
    return response;
}

std::vector<Map> Methods::GetMapList(int limit, int index)
{
    std::vector<Map> list = std::vector<Map>();

    GbxParameters* params = new GbxParameters();
    params->Put(&limit);
    params->Put(&index);
    GbxMessage* getMapList = new GbxMessage("GetMapList", params);
    if(server->Query(getMapList))
    {
        delete getMapList; getMapList = NULL;
        delete params; params = NULL;

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
