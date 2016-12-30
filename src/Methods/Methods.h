#ifndef METHODS_H_
#define METHODS_H_

#include "../GbxRemote/GbxRemote.h"
#include "../Objects/Player.h"
#include "Structs.h"

//* Methods
/**
 * \brief Contains all server methods and returns usable data types.
 */
class Methods
{
public:
    /*!
     * \brief Constructor with link to server connection.
     *
     * \param serverPtr Pointer to server connection.
     */
    Methods(GbxRemote* serverPtr);

    /*!
     * \brief Authenticates with the server. Returns whether authentication was successful.
     *
     * \param username Server username (f.e. SuperAdmin).
     * \param password Server password.
     */
    bool Authenticate(std::string username, std::string password);

    /*!
     * \brief Changes authentication passwordd. Returns whether change was successful.
     *
     * \param username Server username (f.e. SuperAdmin).
     * \param password New password for user.
     */
    bool ChangeAuthPassword(std::string username, std::string password);

    /*!
     * \brief Enables/disables callbacks. Returns whether change was successful.
     *
     * \param enable   Enable (true) or disable (false).
     */
    bool EnableCallbacks(bool enable);

    /*!
     * \brief Sets the API version of the server. Returns whether change was successful.
     *
     * \param version  API version.
     */
    bool SetApiVersion(std::string version);

    /*!
     * \brief Returns list of available server methods.
     *
     * Returns empty vector if no methods could be retrieved.
     */
    std::vector<std::string> ListMethods();

    /*!
     * \brief Returns server version information.
     *
     * Returns empty struct if no information could be retrieved.
     */
    ServerVersion GetVersion();

    /*!
     * \brief Returns system information.
     *
     * Returns empty struct if no information could be retrieved.
     */
    SystemInfo GetSystemInfo();

    /*!
     * \brief Returns server status.
     *
     * Returns empty struct if no information could be retrieved.
     */
    ServerStatus GetStatus();

    /*!
     * \brief Quit the server.
     */
    bool QuitGame();

    /*!
     * \brief Send chat message to all players.
     *
     * \param text        Chat message.
     */
    bool ChatSendServerMessage(std::string text);

    /*!
     * \brief Send chat message to player login.
     *
     * \param text        Chat message.
     * \param login       Player login to which the message should be send.
     */
    bool ChatSendServerMessageToLogin(std::string text, std::string login);

    /*!
     * \brief Send chat message to player.
     *
     * \param text        Chat message.
     * \param login       Player object to which the message should be send.
     */
    bool ChatSendServerMessageToPlayer(std::string text, Player player);

    /*!
     * \brief Get latest chat lines.
     */
    std::vector<std::string> GetChatLines();

    /*!
     * \brief Enable/disable manual chat routing.
     *
     * \param enable      Enable routing?
     * \param autoForward Autoforward?
     */
    bool ChatEnableManualRouting(bool enable, bool autoForward);

    /*!
     * \brief Forwards chat message to player.
     *
     * \param text        Chat message.
     * \param sender      Sending login.
     * \param destination Destination login (empty for all players).
     */
    bool ChatForwardToLogin(std::string text, std::string sender, std::string destination = "");

    /*!
     * \brief Send notice to all players.
     *
     * \param text        Chat message.
     * \param avatarLogin Avatar login.
     * \param Variant     Notice variant (0 = normal, 1 = sad, 2 = happy).
     */
    bool SendNotice(std::string text, std::string avatarLogin = "", int variant = 0);

    /*!
     * \brief Send notice to player login.
     *
     * \param login       Player login.
     * \param text        Chat message.
     * \param avatarLogin Avatar login.
     * \param Variant     Notice variant (0 = normal, 1 = sad, 2 = happy).
     */
    bool SendNoticeToLogin(std::string login, std::string text, std::string avatarLogin = "", int variant = 0);

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
