#include "GbxResponse.h"

#include <cstring>
#include <iostream>

GbxResponse::~GbxResponse()
{
    delete fault;
    fault = NULL;
}

GbxError* GbxResponse::GetFault()
{
    if(fault->number == 0)
    {
        return NULL;
    }

    return fault;
}

void GbxResponse::SetRaw(std::string response)
{
    data = response;
    extractParameters();
}

void GbxResponse::extractParameters()
{
    if(data == "")
        return;

    pugi::xml_document pugiDoc;
    pugi::xml_parse_result pugiResult = pugiDoc.load_string(data.c_str());
    pugi::xml_node pugiMethodResponse = pugiDoc.child("methodResponse");
    std::string pugiResponseType(pugiMethodResponse.first_child().name());
    if(pugiResponseType.find("params") != std::string::npos)
    {
        pugi::xml_node pugiParams = pugiMethodResponse.child("params");
        for (pugi::xml_node pugiChild = pugiParams.first_child(); pugiChild; pugiChild = pugiChild.next_sibling())
        {
            pugi::xml_node pugiValue = pugiChild.child("value");
            GbxResponseParameter responseParam = extractParam(pugiValue);
            parameters.push_back(responseParam);
        }
    }
    else if(pugiResponseType.find("fault") != std::string::npos)
    {
        std::map<std::string, GbxResponseParameter> map = std::map<std::string, GbxResponseParameter>();

        pugi::xml_node faultElement = pugiMethodResponse.child("fault").child("value").child("struct");
        for (pugi::xml_node member = faultElement.first_child(); member; member = member.next_sibling())
        {
            pugi::xml_node name = member.child("name");
            pugi::xml_node value = member.child("value").first_child();
            GbxResponseParameter gbxValue = GbxResponseParameter();
            gbxValue.Type = value.name();
            gbxValue.Text = value.child_value();

            map.insert(std::pair<std::string, GbxResponseParameter>(name.child_value(), gbxValue));

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
        resParam.Struct = map;
        parameters.push_back(resParam);
    }
}
