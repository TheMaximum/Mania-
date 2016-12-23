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
 *
 * Mania++ is a Server Controller for TrackMania 2 servers, written in C++.
 * Copyright (C) 2016 Max Klaversma <maxklaversma@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
     * \brief Prints information about the server.
     */
    void PrintServerInfo();

    /*!
     * \brief Performing our tasks ...
     */
    void MainLoop();

    /*!
     * \brief Disconnects the software from the server.
     */
    void Terminate();

    void PrintParameter(GbxResponseParameter parameter, int paramId, std::string spaces = "    ");

private:
    Config* config;                         /**< \brief Contains the configuration for the software. */
    Logging* logging;                       /**< \brief Contains functions to log information. */
    GbxRemote* server;                      /**< \brief Contains the connection with the server and its methods. */
    std::map<std::string, Player>* players; /**< \brief Contains the list of players currently on the server. */

    /*!
     * \brief Gets a list of current players from the server.
     */
    void retrievePlayerList();
};

#endif // MANIAPP_H_
