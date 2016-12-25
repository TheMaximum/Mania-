#include "GbxServerResponse.h"

#include <string.h>
#include <iostream>

GbxServerResponse::GbxServerResponse()
{
    data = (char*)"";
}

GbxServerResponse::~GbxServerResponse()
{
    //delete parameters;
    //parameters = NULL;
}

void GbxServerResponse::SetRaw(std::string response)
{
    data = response;
}

std::string GbxServerResponse::GetRaw()
{
    return data;
}

std::vector<GbxResponseParameter> GbxServerResponse::GetParameters()
{
    return parameters;
}

GbxResponseParameter GbxServerResponse::extractParam(pugi::xml_node param)
{
    GbxResponseParameter resParam = GbxResponseParameter();
    pugi::xml_node value = param;

    for (pugi::xml_node sibling = value.first_child(); sibling; sibling = sibling.next_sibling())
    {
        std::string valueType(sibling.name());
        resParam.Type = valueType;

        if(valueType.find("array") != std::string::npos)
        {
            std::vector<GbxResponseParameter> arrayData = std::vector<GbxResponseParameter>();

            pugi::xml_node data = sibling.child("data");
            for (pugi::xml_node arrayValue = data.first_child(); arrayValue; arrayValue = arrayValue.next_sibling())
            {
                GbxResponseParameter arrayParam = extractParam(arrayValue);
                arrayData.push_back(arrayParam);
            }

            resParam.Array = arrayData;
        }
        else if(valueType.find("struct") != std::string::npos)
        {
            std::map<std::string, GbxResponseParameter> map = std::map<std::string, GbxResponseParameter>();

            for (pugi::xml_node member = sibling.first_child(); member; member = member.next_sibling())
            {
                pugi::xml_node name = member.child("name");
                GbxResponseParameter structParam = extractParam(member.child("value"));
                map.insert(std::pair<std::string, GbxResponseParameter>(name.child_value(), structParam));
            }

            resParam.Struct = map;
        }
        else
        {
            if(valueType.find("i4") != std::string::npos)
            {
                resParam.Type = "int";
            }

            /*char* paramValue = (char*)sibling.child_value();
            char* value = (char*)malloc(strlen(paramValue) + 1);
            strcpy(value, paramValue);
            resParam.Value = value;*/
            std::string paramValue = (std::string)sibling.child_value();
            resParam.Text = paramValue;
        }
    }

    return resParam;
}
