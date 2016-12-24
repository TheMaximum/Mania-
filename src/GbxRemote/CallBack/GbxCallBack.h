#ifndef GBXCALLBACK_H_
#define GBXCALLBACK_H_

#include "../ServerResponse/GbxServerResponse.h"

//* GbxCallBack
/**
 * \brief CallBack from server, de-XML-fies the callback.
 */
class GbxCallBack : public GbxServerResponse
{
public:
    /*!
     * Returns the method name.
     */
    std::string GetMethodName();

    /*!
     * \brief Sets the raw message value.
     *
     * \param callback Raw callback from server (XML).
     */
    void SetRaw(std::string callback);

private:
    std::string methodName; /**< \brief Method name. */

    /*!
     * \brief Extracts parameters from the raw data (XML).
     */
    void extractParameters();
};

#endif // GBXCALLBACK_H_
