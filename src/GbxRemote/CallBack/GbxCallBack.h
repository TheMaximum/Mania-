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
    char* GetMethodName();

    /*!
     * \brief Sets the raw message value.
     *
     * \param callback Raw callback from server (XML).
     */
    void SetRaw(char* callback);

private:
    char* methodName; /**< \brief Method name. */

    /*!
     * \brief Extracts parameters from the raw data (XML).
     */
    void extractParameters();
};

#endif // GBXCALLBACK_H_
