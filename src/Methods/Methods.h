#ifndef METHODS_H_
#define METHODS_H_

#include "../GbxRemote/GbxRemote.h"

class Methods
{
public:
    Methods(GbxRemote* serverPtr);

    bool Authenticate(std::string username, std::string password);
    bool ChangeAuthPassword(std::string username, std::string password);
    bool EnableCallbacks(bool enable);
    bool SetApiVersion(std::string version);

    // GetVersion();
    // GetStatus();
    // QuitServer();

    // bool CallVote(std::string xmlRequest);
    // bool CallVoteEx(std::string xmlRequest, double ratio, int timeout, int whoVotes);
    // bool CancelVote();
    // struct{CallerLogin, CmdName, CmdParam} GetCurrentCallVote();
    // bool SetCallVoteTimeOut(int timeout);
    // array[CurrentValue, NextValue] GetCallVoteTimeOut();
    // bool SetCallVoteRatio(double ratio);
    // double GetCallVoteRatio();
    // bool SetCallVoteRatios(array[ratios]);
    // array[ratios] GetCallVoteRatios();
    // bool SetCallVoteRatiosEx(bool replaceAll, array[struct{Command, Param, Ratio}]);
    // array[struct{Command, Param, Ratio}] GetCallVoteRatiosEx();

    bool ChatSendServerMessage(std::string message);
    bool ChatSendServerMessageToLogin(std::string message, std::string login);
    // array[std::string] GetChatLines();
    bool ChatEnableManualRouting(bool enable, bool autoForward);
    bool ChatForwardToLogin(std::string message, std::string sender, std::string destination = "");

    bool SendNotice(std::string message, std::string avatarLogin = "", int variant = 0);
    bool SendNoticeToLogin(std::string login, std::string message, std::string avatarLogin = "", int variant = 0);

    bool SendDisplayManialinkPage(std::string page, int timeout, bool hideOnClick);
    bool SendDisplayManialinkPageToLogin(std::string login, std::string page, int timeout, bool hideOnClick);

    bool SendHideManialinkPage();
    bool SendHideManialinkPageToLogin(std::string login);

    // array[struct{Login,PlayerId,Result}] GetManialinkPageAnswers();

    bool SendOpenLinkToLogin(std::string login, std::string link, int linkType);

    bool Kick(std::string login, std::string message = "");
    bool Ban(std::string login, std::string message = "");
    bool BanAndBlackList(std::string login, std::string message, bool save = false);
    bool UnBan(std::string login);
    bool CleanBanList();
    //array[struct{Login,ClientName,IPAddress}] GetBanList(int amount, int index);
    bool BlackList(std::string login);
    bool UnBlackList(std::string login);
    bool CleanBlackList();
    //array[struct{Login}] GetBlackList(int amount, int index);
    bool LoadBlackList(std::string file);
    bool SaveBlackList(std::string file);

    bool AddGuest(std::string login);
    bool RemoveGuest(std::string login);
    bool CleanGuestList();
    //array[struct{Login}] GetGuestList(int amount, int index);
    bool LoadGuestList(std::string file);
    bool SaveGuestList(std::string file);

    bool SetBuddyNotification(std::string login, bool enabled);
    bool GetBuddyNotification(std::string login);

    // bool WriteFile(std::string file, base64 content);

    bool Echo(std::string value1, std::string value2);

    bool Ignore(std::string login);
    bool UnIgnore(std::string login);
    bool CleanIgnoreList();
    // array[struct{Login}] GetIgnoreList(int amount, int index);

    int Pay(std::string login, int cost, std::string label);
    int SendBill(std::string loginFrom, int cost, std::string label, std::string loginTo = "");
    // struct{State, StateName, TransactionId} GetBillState(int billId);
    int GetServerPlanets();

    //struct{?} GetSystemInfo();
    bool SetConnectionRates(int download, int upload);

    //array[struct{Name, Value}] GetServerTags();
    bool SetServerTag(std::string name, std::string value);
    bool UnsetServerTag(std::string name);
    bool ResetServerTags();

    bool SetServerName(std::string name);
    std::string GetServerName();
    bool SetServerComment(std::string comment);
    std::string GetServerComment();
    bool SetHideServer(int setting);
    int GetHideServer();
    bool IsRelayServer();
    bool SetServerPassword(std::string password);
    std::string GetServerPassword();
    bool SetServerPasswordForSpectator(std::string password);
    std::string GetServerPasswordForSpectator();

    bool SetMaxPlayers(int limit);
    int GetMaxPlayers();
    bool SetMaxSpectators(int limit);
    int GetMaxSpectators();

    bool SetLobbyInfo(bool isLobby, int numPlayers, int maxPlayers, double averageLevel);
    //struct{IsLobby,LobbyPlayers,LobbyMaxPlayers,LobbyPlayersLevel} GetLobbyInfo();

    bool CustomizeQuitDialog(std::string manialink, std::string sendToServer, bool proposeAddToFavourites, int delayQuitButton);

    bool KeepPlayerSlots(bool keepPlayerSlots);
    bool IsKeepingPlayerSlots();

    bool EnableP2PUpload(bool enable);
    bool IsP2PUpload();
    bool EnableP2PDownload(bool enable);
    bool IsP2PDownload();
    bool AllowMapDownload(bool allow);
    bool IsMapDownloadAllowed();

    std::string GameDataDirectory();
    std::string GetMapsDirectory();
    std::string GetSkinsDirectory();



private:
    GbxRemote* server;
};

#endif // METHODS_H_
