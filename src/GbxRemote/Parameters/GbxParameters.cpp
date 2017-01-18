#include "GbxParameters.h"
#include "GbxParameter.h"

void GbxStructParameters::Put(std::string methodName, Parameter parameter)
{
    std::string param = GbxParameter(parameter).GetXml();

    std::stringstream parameterStream;
    parameterStream << "<member><name>" << methodName << "</name><value>" << param << "</value></member>";
    parameters.push_back(parameterStream.str());
}
