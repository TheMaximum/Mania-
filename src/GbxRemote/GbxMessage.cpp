#include "GbxRemote.h"

GbxMessage::GbxMessage(std::string method, std::vector<void*>* params = new std::vector<void*>())
    : method(method)
{
    xml  = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    xml += "<methodCall>";
    xml += "<methodName>" + method + "</methodName>";
    xml += "<params>";
    for(int paramId = 0; paramId < params->size(); paramId++)
    {
        /*std::string* paramPtr = static_cast<std::string*>(params->at(paramId));
        std::string  param = *paramPtr;
        std::cout << "PARAM: " << param << std::endl;

        xml += "<param><value><string>";
        xml += param;
        xml += "</string></value></param>";*/

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
