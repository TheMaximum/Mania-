#include "GbxMessage.h"

GbxMessage::GbxMessage(std::string methodName, GbxParameters* parameters)
    : method(methodName)
{
    xml  = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    xml += "<methodCall>";
    xml += "<methodName>" + method + "</methodName>";
    xml += "<params>";
    if(parameters != NULL)
    {
        for(int paramId = 0; paramId < parameters->GetParameters().size(); paramId++)
        {
            xml += "<param><value>";
            GbxParameter param = GbxParameter(parameters->GetParameters().at(paramId));
            xml += param.GetXml();
            xml += "</value></param>";

            //delete param; param = NULL;
        }
    }
    xml += "</params>";
    xml += "</methodCall>";
}

GbxMessage::GbxMessage(std::string methodName, GbxParameters parameters)
    : method(methodName)
{
    xml  = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    xml += "<methodCall>";
    xml += "<methodName>" + method + "</methodName>";
    xml += "<params>";
    for(int paramId = 0; paramId < parameters.GetParameters().size(); paramId++)
    {
        xml += "<param><value>";
        GbxParameter param = GbxParameter(parameters.GetParameters().at(paramId));
        xml += param.GetXml();
        xml += "</value></param>";

        //delete param; param = NULL;
    }
    xml += "</params>";
    xml += "</methodCall>";
}

std::string GbxMessage::GetMethod()
{
    return method;
}

std::string GbxMessage::GetXml()
{
    return xml;
}
