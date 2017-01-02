#include "../Methods.h"

bool Methods::AddMap(std::string fileName)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&fileName);

    if(server->Query(GbxMessage("AddMap", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

int Methods::AddMapList(std::vector<std::string> fileNames)
{
    int response = 0;

    GbxParameters params = GbxParameters();
    params.Put(&fileNames);

    if(server->Query(GbxMessage("AddMapList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = atoi(responseParams.at(0).GetString().c_str());
    }

    return response;
}

bool Methods::RemoveMap(std::string fileName)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&fileName);

    if(server->Query(GbxMessage("RemoveMap", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

int Methods::RemoveMapList(std::vector<std::string> fileNames)
{
    int response = 0;

    GbxParameters params = GbxParameters();
    params.Put(&fileNames);

    if(server->Query(GbxMessage("RemoveMapList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = atoi(responseParams.at(0).GetString().c_str());
    }

    return response;
}

bool Methods::InsertMap(std::string fileName)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&fileName);

    if(server->Query(GbxMessage("InsertMap", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

int Methods::InsertMapList(std::vector<std::string> fileNames)
{
    int response = 0;

    GbxParameters params = GbxParameters();
    params.Put(&fileNames);

    if(server->Query(GbxMessage("InsertMapList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = atoi(responseParams.at(0).GetString().c_str());
    }

    return response;
}

bool Methods::ChooseNextMap(std::string fileName)
{
    bool response = false;

    GbxParameters params = GbxParameters();
    params.Put(&fileName);

    if(server->Query(GbxMessage("ChooseNextMap", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        std::istringstream(responseParams.at(0).GetString()) >> response;
    }

    return response;
}

int Methods::ChooseNextMapList(std::vector<std::string> fileNames)
{
    int response = 0;

    GbxParameters params = GbxParameters();
    params.Put(&fileNames);

    if(server->Query(GbxMessage("ChooseNextMapList", params)))
    {
        std::vector<GbxResponseParameter> responseParams = server->GetResponse()->GetParameters();
        response = atoi(responseParams.at(0).GetString().c_str());
    }

    return response;
}
