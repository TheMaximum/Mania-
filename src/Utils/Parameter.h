#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <typeinfo>
#include <cxxabi.h>

//* Parameter
/**
 * \brief Pointer and type information of a parameter.
 */
struct Parameter
{
    void* pointer;                  /**< \brief Pointer to the parameter. */
    const std::type_info* typeinfo; /**< \brief Type information of the parameter. */

    /*!
     * \brief Updates properties with input information.
     *
     * \param input Input to be parameterized.
     */
    template<typename T>
    Parameter(T* input)
    {
        pointer = input;
        typeinfo = &typeid(T);
    }

    /*!
     * \brief Updates properties with input information.
     *
     * \param input Pointer to content.
     * \param type  Pointer type.
     */
    Parameter(void* input, const std::type_info* type)
    {
        pointer = input;
        typeinfo = type;
    }

    /*!
     * \brief Returns demangled type information.
     */
    std::string GetType()
    {
        int status;
        char* realParamName = abi::__cxa_demangle(typeinfo->name(), 0, 0, &status);
        std::string paramType = std::string(realParamName);

        free(realParamName);
        realParamName = NULL;

        return paramType;
    }
};

#endif // PARAMETER_H_
