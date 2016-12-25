#include "GbxParameter.h"

GbxParameter::GbxParameter(GbxParam param)
{
    determineType(param.typeinfo);
    dereferenceData(param.pointer);
}

std::string GbxParameter::GetXml()
{
    std::string xml;
    xml += "<" + xmlType + ">";
    xml += data;
    xml += "</" + xmlType + ">";

    return xml;
}

void GbxParameter::determineType(const std::type_info* param)
{
    int status;
    char* realParamName = abi::__cxa_demangle(param->name(), 0, 0, &status);
    type = std::string(realParamName);

    if(type.find("GbxParameters*") != std::string::npos)
    {
        xmlType = "array";
    }
    else if(type.find("string") != std::string::npos ||
            type.find("char*") != std::string::npos ||
            type.find("char [") != std::string::npos)
    {
        xmlType = "string";
    }
    else if(type.find("i4") != std::string::npos ||
            type.find("int") != std::string::npos ||
            type.find("long") != std::string::npos ||
            type.find("short") != std::string::npos)
    {
        xmlType = "int";
    }
    else if(type.find("double") != std::string::npos ||
            type.find("float") != std::string::npos)
    {
        xmlType = "double";
    }
    else if(type.find("bool") != std::string::npos)
    {
        xmlType = "boolean";
    }
    else
    {
        std::cout << "[WARNING] Undefined type, name: " << type << std::endl;
    }

    std::free(realParamName); realParamName = NULL;
}

void GbxParameter::dereferenceData(void* pointer)
{
    if(type.find("GbxParameters*") != std::string::npos)
    {
        data += "<data>";
        GbxParameters** paramPtr = static_cast<GbxParameters**>(pointer);
        GbxParameters* parameters = *paramPtr;
        std::vector<GbxParam>* gbxParams = parameters->GetParameters();
        for(int paramId = 0; paramId < gbxParams->size(); paramId++)
        {
            data += "<value>";
            GbxParameter* param = new GbxParameter(gbxParams->at(paramId));
            data += param->GetXml();
            data += "</value>";
        }
        data += "</data>";
    }
    else if(type.find("char [") != std::string::npos)
    {
        char* paramPtr = static_cast<char*>(pointer);
        data = std::string(paramPtr);
    }
    else if(type.find("int") != std::string::npos ||
            type.find("i4") != std::string::npos)
    {
        int* paramPtr = static_cast<int*>(pointer);
        int param = *paramPtr;
        data = std::to_string(param);
    }
    else if(type.find("long") != std::string::npos)
    {
        long* paramPtr = static_cast<long*>(pointer);
        long param = *paramPtr;
        data = std::to_string(param);
    }
    else if(type.find("short") != std::string::npos)
    {
        short* paramPtr = static_cast<short*>(pointer);
        short param = *paramPtr;
        data = std::to_string(param);
    }
    else if(type.find("double") != std::string::npos)
    {
        double* paramPtr = static_cast<double*>(pointer);
        double param = *paramPtr;
        data = std::to_string(param);
    }
    else if(type.find("float") != std::string::npos)
    {
        float* paramPtr = static_cast<float*>(pointer);
        float param = *paramPtr;
        data = std::to_string(param);
    }
    else if(type.find("bool") != std::string::npos)
    {
        bool* paramPtr = static_cast<bool*>(pointer);
        bool param = *paramPtr;
        data = std::to_string(param);
    }
    else
    {
        std::string* paramPtr = static_cast<std::string*>(pointer);
        data = paramPtr->c_str();
    }
}
