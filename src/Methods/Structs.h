#ifndef STRUCTS_H_
#define STRUCTS_H_

struct ServerVersion
{
    /*
        Parameter #0: 2013-04-16 (string) (ApiVersion)
        Parameter #1: 2015-06-16_18_00 (string) (Build)
        Parameter #2: ManiaPlanet (string) (Name)
        Parameter #3: TMCanyon (string) (TitleId)
        Parameter #4: 3.3.0 (string) (Version)
    */
    std::string ApiVersion;
    std::string Build;
    std::string Name;
    std::string TitleId;
    std::string Version;
};

struct SystemInfo
{
    /*
        Parameter #0: 100000 (int) (ConnectionDownloadRate)
        Parameter #1: 50000 (int) (ConnectionUploadRate)
        Parameter #2: 1 (boolean) (IsDedicated)
        Parameter #3: 1 (boolean) (IsServer)
        Parameter #4: 3490 (int) (P2PPort)
        Parameter #5: 2390 (int) (Port)
        Parameter #6: 178.63.88.110 (string) (PublishedIp)
        Parameter #7: testsmurfer (string) (ServerLogin)
        Parameter #8: 0 (int) (ServerPlayerId)
        Parameter #9: TMCanyon (string) (TitleId)
    */
    int ConnectionDownloadRate;
    int ConnectionUploadRate;
    bool IsDedicated;
    bool IsServer;
    int P2PPort;
    int Port;
    std::string PublishedIp;
    std::string ServerLogin;
    int ServerPlayerId;
    std::string TitleId;
};

#endif // STRUCTS_H_
