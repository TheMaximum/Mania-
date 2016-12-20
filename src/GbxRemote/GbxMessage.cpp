#include "GbxMessage.h"
#include "GbxParameter.h"

#include <iostream>

GbxMessage::GbxMessage(std::string method, GbxParameters* parameters)
    : method(method)
{
    xml  = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    xml += "<methodCall>";
    xml += "<methodName>" + method + "</methodName>";
    xml += "<params>";
    if(parameters != NULL)
    {
        for(int paramId = 0; paramId < parameters->GetParameters()->size(); paramId++)
        {
            xml += "<param><value>";
            GbxParameter* param = new GbxParameter(parameters->GetParameters()->at(paramId));
            xml += param->GetXml();
            xml += "</value></param>";
        }
    }
    xml += "</params>";
    xml += "</methodCall>";

    std::cout << "SENDING XML: " << xml << std::endl;
}

std::string GbxMessage::GetXml()
{
    return xml;
}
