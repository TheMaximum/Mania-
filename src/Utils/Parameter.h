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
