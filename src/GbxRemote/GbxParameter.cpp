/*class GbxParameter
{
public:
    GbxParameter(void*);
    std::string GetXml();

private:
    std::string data;
    std::string type;
    std::string calculateType();
};*/

#include "GbxRemote.h"

GbxParameter::GbxParameter(void* param)
{
    std::string* paramPtr = static_cast<std::string*>(param);
    data = *paramPtr;

    type = calculateType(param);
}

std::string GbxParameter::GetXml()
{
    std::string xml;
    xml += "<param><value><" + type + ">";
    xml += data;
    xml += "</" + type + "></value></param>";

    return xml;
}

std::string GbxParameter::calculateType(void* param)
{
    /*if(dynamic_cast<bool*>(param) != NULL)
    {
        return "boolean";
    }
    if(static_cast<int*>(param) != NULL)
    {
        return "int";
    }
    if(static_cast<double*>(param) != NULL)
    {
        return "double";
    }*/

    return "string";
}
