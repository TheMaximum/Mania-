#include "GbxResponse.h"

#include <typeinfo>

GbxResponse::GbxResponse()
{
    data = (char*)"";
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

std::vector<GbxResponseParameter> GbxResponse::GetParameters()
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
        GbxResponseParameter* responseParamPtr = extractParam(child);
        GbxResponseParameter responseParam = *responseParamPtr;
        std::cout << "ADDING: " << responseParamPtr->Type << " => " << (responseParam.GetStringPtr())->c_str() << std::endl;
        parameters.push_back(responseParam);

        /*tinyxml2::XMLElement* value = child->FirstChildElement("value");

        for (tinyxml2::XMLElement* sibling = value->FirstChildElement(); sibling != NULL; sibling = sibling->NextSiblingElement())
        {
            std::string valueType(sibling->Name());
            if(valueType.find("array") != std::string::npos)
            {
                std::cout << sibling->Name() << std::endl;
                std::string substitute = "HALLO";
                std::cout << "ADDING: " << valueType << " => " << substitute << std::endl;
                parameters.push_back({ valueType, &substitute });
            }
            else
            {
                std::string value(sibling->GetText());
                std::cout << "ADDING: " << valueType << " => " << value << std::endl;
                parameters.push_back({ valueType, &value });
            }
        }*/
    }
}

GbxResponseParameter* GbxResponse::extractParam(tinyxml2::XMLElement* param)
{
    GbxResponseParameter* resParam = new GbxResponseParameter();
    tinyxml2::XMLElement* value = param->FirstChildElement("value");

    for (tinyxml2::XMLElement* sibling = value->FirstChildElement(); sibling != NULL; sibling = sibling->NextSiblingElement())
    {
        std::string valueType(sibling->Name());
        if(valueType.find("array") != std::string::npos)
        {
            std::cout << sibling->Name() << std::endl;
            std::string substitute = "HALLO";
            //return { valueType, &substitute };
            resParam->Type = valueType;
            resParam->Value = &substitute;
        }
        else
        {
            std::string value(sibling->GetText());
            resParam->Type = valueType;
            resParam->Value = &value; // = { valueType, &value };
            std::cout << "ADDING: " << resParam->Type << " => " << resParam->GetString() << std::endl;
            //return { valueType, &value };
        }
    }

    return resParam;
}
