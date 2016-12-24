#ifndef GBXRESPONSE_H_
#define GBXRESPONSE_H_

#include "../ServerResponse/GbxServerResponse.h"

//* GbxResponse
/**
 * \brief Response from server, de-XML-fies the response.
 */
class GbxResponse : public GbxServerResponse
{
public:
    /*!
     * Returns whether the response is a fault.
     */
    GbxError* GetFault();

    /*!
     * \brief Sets the raw message value.
     *
     * \param response Raw response from server (XML).
     */
    void SetRaw(std::string response);

private:
    GbxError* fault = new GbxError; /**< \brief Current fault (error). */

    /*!
     * \brief Extracts parameters from the raw data (XML).
     */
    void extractParameters();
};

#endif // GBXRESPONSE_H_
