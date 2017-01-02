#ifndef GBXPARAMETERS_H_
#define GBXPARAMETERS_H_

#include <string>
#include <vector>
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

//* GbxParameters
/**
 * \brief List of parameters.
 */
class GbxParameters
{
public:
    /*!
     * \brief Deletes + nullifies parameters pointer.
     */
    ~GbxParameters()
    {
        //delete parameters;
        //parameters = NULL;
    }

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
