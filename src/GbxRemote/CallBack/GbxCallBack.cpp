#include "GbxCallBack.h"

#include <cstring>
#include <iostream>

std::string GbxCallBack::GetMethodName()
{
    return methodName;
}

void GbxCallBack::SetRaw(char* callback)
{
    data = callback;
    extractParameters();
}

void GbxCallBack::extractParameters()
{
    if(data == "")
        return;

    pugi::xml_document pugiDoc;
    pugi::xml_parse_result pugiResult = pugiDoc.load_string(data);
    pugi::xml_node pugiMethodResponse = pugiDoc.first_child();

    pugi::xml_node methodResponse = pugiDoc.child("methodCall");
    std::string responseType(methodResponse.last_child().name());
    if(responseType.find("params") != std::string::npos)
    {
        methodName = (std::string)methodResponse.first_child().child_value();
        pugi::xml_node params = methodResponse.child("params");

        for (pugi::xml_node child = params.first_child(); child; child = child.next_sibling())
        {
            pugi::xml_node value = child.child("value");
            GbxResponseParameter responseParam = extractParam(value);
            parameters->push_back(responseParam);
        }
    }
}
