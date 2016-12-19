#include "GbxMessage.h"
#include "GbxParameter.h"

#include <iostream>

GbxMessage::GbxMessage(std::string method, GbxParameters* parameters = NULL)
    : method(method)
{
    xml  = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    xml += "<methodCall>";
    xml += "<methodName>" + method + "</methodName>";
    xml += "<params>";
    for(int paramId = 0; paramId < parameters->GetParameters()->size(); paramId++)
    {
        xml += "<param><value>";
        GbxParameter* param = new GbxParameter(parameters->GetParameters()->at(paramId));
        xml += param->GetXml();
        xml += "</value></param>";
    }
    xml += "</params>";
    xml += "</methodCall>";
}

std::string GbxMessage::GetXml()
{
    return xml;
}
