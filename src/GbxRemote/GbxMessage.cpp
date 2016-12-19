#include "GbxMessage.h"
#include "GbxParameter.h"

GbxMessage::GbxMessage(std::string method, std::vector<void*>* params = new std::vector<void*>())
    : method(method)
{
    xml  = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    xml += "<methodCall>";
    xml += "<methodName>" + method + "</methodName>";
    xml += "<params>";
    for(int paramId = 0; paramId < params->size(); paramId++)
    {
        GbxParameter param = new GbxParameter(params->at(paramId));
        xml += param.GetXml();
    }
    xml += "</params>";
    xml += "</methodCall>";
}

std::string GbxMessage::GetXml()
{
    return xml;
}
