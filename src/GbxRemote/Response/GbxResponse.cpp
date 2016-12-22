#include "GbxResponse.h"

#include <iostream>

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

    pugi::xml_document pugiDoc;
    pugi::xml_parse_result pugiResult = pugiDoc.load_string(data);
    pugi::xml_node pugiMethodResponse = pugiDoc.first_child();
    std::string pugiResponseType(pugiMethodResponse.first_child().name());
    if(pugiResponseType.find("params") != std::string::npos)
    {
        pugi::xml_node pugiParams = pugiMethodResponse.child("params");
        for (pugi::xml_node pugiChild = pugiParams.first_child(); pugiChild; pugiChild = pugiChild.next_sibling())
        {
            pugi::xml_node pugiValue = pugiChild.child("value");
            GbxResponseParameter responseParam = extractParam(pugiValue);
            parameters->push_back(responseParam);
        }
    }
    else if(pugiResponseType.find("fault") != std::string::npos)
    {
        std::map<std::string, GbxResponseParameter>* map = new std::map<std::string, GbxResponseParameter>();

        pugi::xml_node faultElement = pugiMethodResponse.child("fault").child("value").child("struct");
        for (pugi::xml_node member = faultElement.first_child(); member; member = member.next_sibling())
        {
            pugi::xml_node name = member.child("name");
            pugi::xml_node value = member.child("value").first_child();
            GbxResponseParameter gbxValue = GbxResponseParameter();
            gbxValue.Type = value.name();
            gbxValue.Value = (char*)value.child_value();

            map->insert(std::pair<std::string, GbxResponseParameter>(name.child_value(), gbxValue));

            if(std::string(name.child_value()).find("faultCode") != std::string::npos)
            {
                fault->number = atoi(value.child_value());
            }
            else if(std::string(name.child_value()).find("faultString") != std::string::npos)
            {
                fault->message = value.child_value();
            }
        }

        GbxResponseParameter resParam = GbxResponseParameter();
        resParam.Type = "struct";
        resParam.Value = map;
        parameters->push_back(resParam);
    }

    /*std::cout << "TINYXML2" << std::endl;
    tinyxml2::XMLDocument document;
    document.Parse(response, strlen(response));

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
    }*/
}
