#include "GbxResponse.h"

GbxResponse::GbxResponse()
{
    data = (char*)"";
}

GbxResponse::~GbxResponse()
{
    delete parameters;
    parameters = NULL;
}

void GbxResponse::SetRaw(char* response)
{
    data = response;

    extractParameters();
}

char* GbxResponse::GetRaw()
{
    return data;
}

std::vector<GbxResponseParameter>* GbxResponse::GetParameters()
{
    return parameters;
}

void GbxResponse::extractParameters()
{
    if(data == "")
        return;

    tinyxml2::XMLDocument document;
    document.Parse(data, strlen(data));

    tinyxml2::XMLElement* params = document.FirstChildElement("methodResponse")->FirstChildElement("params");

    for (tinyxml2::XMLElement* child = params->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        tinyxml2::XMLElement* value = child->FirstChildElement("value");
        GbxResponseParameter responseParam = extractParam(value);
        parameters->push_back(responseParam);
    }
}

GbxResponseParameter GbxResponse::extractParam(tinyxml2::XMLElement* param)
{
    GbxResponseParameter resParam = GbxResponseParameter();
    tinyxml2::XMLElement* value = param/*->FirstChildElement("value")*/;

    for (tinyxml2::XMLElement* sibling = value->FirstChildElement(); sibling != NULL; sibling = sibling->NextSiblingElement())
    {
        std::string valueType(sibling->Name());
        if(valueType.find("array") != std::string::npos)
        {
            resParam.Type = valueType;
            std::vector<GbxResponseParameter>* arrayData = new std::vector<GbxResponseParameter>();

            tinyxml2::XMLElement* data = sibling->FirstChildElement("data");
            for (tinyxml2::XMLElement* arrayValue = data->FirstChildElement(); arrayValue != NULL; arrayValue = arrayValue->NextSiblingElement())
            {
                GbxResponseParameter arrayParam = extractParam(arrayValue);
                arrayData->push_back(arrayParam);
            }

            resParam.Value = arrayData;
        }
        else
        {
            char* value = (char*)sibling->GetText();
            resParam.Type = valueType;
            resParam.Value = value;
        }
    }

    return resParam;
}
