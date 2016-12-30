#ifndef METHODS_STRUCTS_H_
#define METHODS_STRUCTS_H_

//* ServerInfo
/**
 * \brief Struct with server version information.
 */
struct ServerVersion
{
    std::string ApiVersion; /**< \brief API version (f.e. 2013-04-06). */
    std::string Build;      /**< \brief Build version (f.e. 2015-06-06_18_00). */
    std::string Name;       /**< \brief Platform name (f.e. ManiaPlanet). */
    std::string TitleId;    /**< \brief Title identifier (f.e. TMCanyon). */
    std::string Version;    /**< \brief Platform version (f.e. 3.3.0). */
};

//* SystemInfo
/**
 * \brief Struct with system information.
 */
struct SystemInfo
{
    int ConnectionDownloadRate; /**< \brief Download rate in Kbps. */
    int ConnectionUploadRate;   /**< \brief Upload rate in Kbps. */
    bool IsDedicated;           /**< \brief Is dedicated server? */
    bool IsServer;              /**< \brief Is server? */
    int P2PPort;                /**< \brief Peer-to-peer serverport. */
    int Port;                   /**< \brief Game serverport. */
    std::string PublishedIp;    /**< \brief Server IP Address. */
    std::string ServerLogin;    /**< \brief Server login. */
    int ServerPlayerId;         /**< \brief Server player identifier. */
    std::string TitleId;        /**< \brief Title identifier. */
};

//* ServerStatus
/**
 * \brief Struct with server status.
 */
struct ServerStatus
{
    int Code;         /**< \brief Current status code. */
    std::string Name; /**< \brief Current status name. */
};

#endif // METHODS_STRUCTS_H_
