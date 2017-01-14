#ifndef GBXPARAMETERS_H_
#define GBXPARAMETERS_H_

#include <string>
#include <vector>
#include <map>
#include <typeinfo>

#include "../../Utils/Parameter.h"

//* GbxStructParameters
/**
 * \brief List of struct parameters.
 */
class GbxStructParameters
{
public:
    /*!
     * \brief Add parameter to the list.
     *
     * \param text Parameter XML.
     */
    void Put(std::string text)
    {
        parameters.push_back(text);
    }

    /*!
     * \brief Return the current list of parameters.
     */
    std::vector<std::string> GetParameters()
    {
        return parameters;
    }

private:
    std::vector<std::string> parameters = std::vector<std::string>(); /**< \brief List of parameters. */
};

//* GbxParameters
/**
 * \brief List of parameters.
 */
class GbxParameters
{
public:
    /*!
     * \brief Add parameter to the list.
     *
     * \param pointer Pointer to the parameter.
     */
    template<typename T>
    void Put(T* pointer)
    {
        Parameter paramStruct = { pointer, &typeid(T) };
        parameters.push_back(paramStruct);
    }

    /*!
     * \brief Return the current list of parameters.
     */
    std::vector<Parameter> GetParameters()
    {
        return parameters;
    }

private:
    std::vector<Parameter> parameters = std::vector<Parameter>(); /**< \brief List of parameters. */
};

#endif // GBXPARAMETERS_H_
