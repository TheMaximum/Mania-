#include "GbxCallBack.h"

#include <iostream>

char* GbxCallBack::GetMethodName()
{
    return methodName;
}

void GbxCallBack::SetRaw(char* callback)
{
    data = callback;
    extractParameters();
}

/*
    <?xml version="1.0" encoding="UTF-8"?>
    <methodCall>
        <methodName>TrackMania.StatusChanged</methodName>
        <params>
            <param><value><i4>3</i4></value></param>
            <param><value><string>Running - Synchronization</string></value></param>
        </params>
    </methodCall>
*/

void GbxCallBack::extractParameters()
{
    if(data == "")
        return;

    tinyxml2::XMLDocument document;
    document.Parse(data, strlen(data));

    tinyxml2::XMLElement* methodResponse = document.FirstChildElement("methodCall");
    std::string responseType(methodResponse->LastChildElement()->Name());
    if(responseType.find("params") != std::string::npos)
    {
        methodName = (char*)methodResponse->FirstChildElement()->GetText();
        tinyxml2::XMLElement* params = methodResponse->FirstChildElement("params");

        for (tinyxml2::XMLElement* child = params->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
        {
            tinyxml2::XMLElement* value = child->FirstChildElement("value");
            GbxResponseParameter responseParam = extractParam(value);
            parameters->push_back(responseParam);
        }
    }
}
