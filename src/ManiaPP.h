#ifndef MANIAPP_H_
#define MANIAPP_H_

#include "Config/Version.h"
#include "Config/Config.h"
#include "GbxRemote/GbxRemote.h"
#include "Utils/Logging.h"

//* ManiaPP
/**
 * \brief Main class.
 */
class ManiaPP
{
public:
    /*!
     * \brief Sets up the several variables needed to run the software.
     */
    ManiaPP();

    /*!
     * \brief Connects and authenticates with the server.
     */
    void ConnectToServer();

    /*!
     * \brief Disconnects the software from the server.
     */
    void Terminate();

private:
    Config* config;    /**< \brief Contains the configuration for the software. */
    Logging* logging;  /**< \brief Contains functions to log information. */
    GbxRemote* server; /**< \brief Contains the connection with the server and its methods. */
};

#endif // MANIAPP_H_
