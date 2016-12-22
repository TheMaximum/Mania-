#include "GbxServerResponse.h"

GbxServerResponse::GbxServerResponse()
{
    data = (char*)"";
}

GbxServerResponse::~GbxServerResponse()
{
    delete parameters;
    parameters = NULL;
}

void GbxServerResponse::SetRaw(char* response)
{
    data = response;
}

char* GbxServerResponse::GetRaw()
{
    return data;
}

std::vector<GbxResponseParameter>* GbxServerResponse::GetParameters()
{
    return parameters;
}

GbxResponseParameter GbxServerResponse::extractParam(tinyxml2::XMLElement* param)
{
    GbxResponseParameter resParam = GbxResponseParameter();
    tinyxml2::XMLElement* value = param/*->FirstChildElement("value")*/;

    for (tinyxml2::XMLElement* sibling = value->FirstChildElement(); sibling != NULL; sibling = sibling->NextSiblingElement())
    {
        std::string valueType(sibling->Name());
        resParam.Type = valueType;

        if(valueType.find("array") != std::string::npos)
        {
            std::vector<GbxResponseParameter>* arrayData = new std::vector<GbxResponseParameter>();

            tinyxml2::XMLElement* data = sibling->FirstChildElement("data");
            for (tinyxml2::XMLElement* arrayValue = data->FirstChildElement(); arrayValue != NULL; arrayValue = arrayValue->NextSiblingElement())
            {
                GbxResponseParameter arrayParam = extractParam(arrayValue);
                arrayData->push_back(arrayParam);
            }

            resParam.Value = arrayData;
        }
        else if(valueType.find("struct") != std::string::npos)
        {
            std::map<std::string, GbxResponseParameter>* map = new std::map<std::string, GbxResponseParameter>();

            for (tinyxml2::XMLElement* member = sibling->FirstChildElement(); member != NULL; member = member->NextSiblingElement())
            {
                tinyxml2::XMLElement* name = member->FirstChildElement("name");
                GbxResponseParameter structParam = extractParam(member->FirstChildElement("value"));

                map->insert(std::pair<std::string, GbxResponseParameter>(name->GetText(), structParam));
            }

            resParam.Value = map;
        }
        else
        {
            char* value = (char*)sibling->GetText();
            resParam.Value = value;
        }
    }

    return resParam;
}
