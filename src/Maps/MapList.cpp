#include "MapList.h"

void MapList::SetCurrentMap(std::string uid)
{
    Map* map = &(List.at(uid));
    Current = map;
}
