#include "MapList.h"

void MapList::SetCurrentMap(std::string uid)
{
    Map* map = &(List.at(uid));
    Current = map;
}

Map* MapList::GetByUid(std::string uid)
{
    Map* result = NULL;

    std::map<std::string, Map>::iterator mapIt = List.find(uid);
    if(mapIt != List.end())
    {
        result = &mapIt->second;
    }

    return result;
}
