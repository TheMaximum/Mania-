#include "GbxResponse.h"

GbxError* GbxResponse::GetFault()
{
    if(fault->number == 0)
    {
        return NULL;
    }

    return fault;
}

void GbxResponse::SetRaw(char* response)
{
    data = response;
    extractParameters();
}

void GbxResponse::extractParameters()
{
    if(data == "")
        return;

    tinyxml2::XMLDocument document;
    document.Parse(data, strlen(data));

    tinyxml2::XMLElement* methodResponse = document.FirstChildElement("methodResponse");
    std::string responseType(methodResponse->FirstChildElement()->Name());
    if(responseType.find("params") != std::string::npos)
    {
        tinyxml2::XMLElement* params = methodResponse->FirstChildElement("params");

        for (tinyxml2::XMLElement* child = params->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
        {
            tinyxml2::XMLElement* value = child->FirstChildElement("value");
            GbxResponseParameter responseParam = extractParam(value);
            parameters->push_back(responseParam);
        }
    }
    else if(responseType.find("fault") != std::string::npos)
    {
        std::map<std::string, GbxResponseParameter>* map = new std::map<std::string, GbxResponseParameter>();

        tinyxml2::XMLElement* faultElement = methodResponse->FirstChildElement("fault")->FirstChildElement("value")->FirstChildElement("struct");
        for (tinyxml2::XMLElement* member = faultElement->FirstChildElement(); member != NULL; member = member->NextSiblingElement())
        {
            tinyxml2::XMLElement* name = member->FirstChildElement("name");
            tinyxml2::XMLElement* value = member->FirstChildElement("value")->FirstChildElement();
            GbxResponseParameter gbxValue = GbxResponseParameter();
            gbxValue.Type = value->Name();
            gbxValue.Value = (char*)value->GetText();

            map->insert(std::pair<std::string, GbxResponseParameter>(name->GetText(), gbxValue));

            if(std::string(name->GetText()).find("faultCode") != std::string::npos)
            {
                fault->number = atoi(value->GetText());
            }
            else if(std::string(name->GetText()).find("faultString") != std::string::npos)
            {
                fault->message = value->GetText();
            }
        }

        GbxResponseParameter resParam = GbxResponseParameter();
        resParam.Type = "struct";
        resParam.Value = map;
        parameters->push_back(resParam);
    }
}
