#ifndef MANIAPP_H_
#define MANIAPP_H_

#include "Config/Version.h"
#include "Config/Config.h"
#include "GbxRemote/GbxRemote.h"
#include "Objects/Player.h"
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
    bool ConnectToServer();

    /*!
     * \brief Performing our tasks ...
     */
    void MainLoop();

    /*!
     * \brief Disconnects the software from the server.
     */
    void Terminate();

private:
    Config* config;               /**< \brief Contains the configuration for the software. */
    Logging* logging;             /**< \brief Contains functions to log information. */
    GbxRemote* server;            /**< \brief Contains the connection with the server and its methods. */
    std::vector<Player>* players; /**< \brief Contains the list of players currently on the server. */

    /*!
     * \brief Gets a list of current players from the server.
     */
    void retrievePlayerList();
};

#endif // MANIAPP_H_
