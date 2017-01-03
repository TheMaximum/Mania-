#ifndef GBXPARAMETERS_H_
#define GBXPARAMETERS_H_

#include <string>
#include <vector>
#include <map>
#include <typeinfo>

//* GbxParam
/**
 * \brief Pointer and type information of a parameter.
 */
struct GbxParam
{
    void* pointer;                  /**< \brief Pointer to the parameter. */
    const std::type_info* typeinfo; /**< \brief Type information of the parameter. */
};

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
        GbxParam paramStruct = { pointer, &typeid(T) };
        parameters.push_back(paramStruct);
    }

    /*!
     * \brief Return the current list of parameters.
     */
    std::vector<GbxParam> GetParameters()
    {
        return parameters;
    }

private:
    std::vector<GbxParam> parameters = std::vector<GbxParam>(); /**< \brief List of parameters. */
};

#endif // GBXPARAMETERS_H_
