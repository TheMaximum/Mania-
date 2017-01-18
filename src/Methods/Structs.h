#ifndef METHODS_STRUCTS_H_
#define METHODS_STRUCTS_H_

//* ServerVersion
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

//* ServerInfo
/**
 * \brief Struct with server information.
 */
struct ServerInfo
{
    std::string Name;
    std::string Comment;
    int MaxPlayers;
    int MaxSpectators;

    std::string ControllerVersion; /**< \brief Version of the server controller. */
    Player Account;                /**< \brief Player object of server. */
    std::string PackMask;          /**< \brief Pack mask. */
    ServerVersion Version;         /**< \brief Struct with server version information. */
    SystemInfo System;             /**< \brief Struct with system information. */
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

//* ManiaLinkPageAnswer
/**
 * \brief Struct with a ManiaLink page answer.
 */
struct ManiaLinkPageAnswer
{
    Player AnsweringPlayer; /**< \brief Player answering. */
    int Result;             /**< \brief Answering result. */
};

//* CurrentCallVote
/**
 * \brief Struct with a current callvote.
 */
struct CurrentCallVote
{
    std::string CallerLogin; /**< \brief Player login who created the vote. */
    std::string CmdName;     /**< \brief Name of the callvote. */
    std::string CmdParam;    /**< \brief Callvote parameter. */
};

//* CallVoteRatio
/**
 * \brief Struct with a callvote ratio.
 */
struct CallVoteRatio
{
    std::string Command; /**< \brief Command for the ratio. */
    double Ratio;        /**< \brief Vote passing ratio (between 0 and 1, -1 for disable). */
};

//* CallVote
/**
 * \brief Struct with a callvote.
 */
struct ExtendedCallVoteRatio
{
    std::string Command; /**< \brief Name of the callvote. */
    std::string Param;   /**< \brief Parameter of the callvote. */
    double Ratio;        /**< \brief Vote passing ratio (between 0 and 1, -1 for disable). */
};

//* CurrentNextValue
/**
 * \brief Struct with a current and next value.
 */
struct CurrentNextValue
{
    int CurrentValue; /**< \brief Current value of the setting. */
    int NextValue;    /**< \brief Next value of the setting. */
};

#endif // METHODS_STRUCTS_H_
