#ifndef METHODS_H_
#define METHODS_H_

#include "../GbxRemote/GbxRemote.h"
#include "../Objects/Map.h"
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
    Methods(GbxRemote* serverPtr, std::map<std::string, Player>* playerListPtr);

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
     * \brief Sets the server connection rates.
     *
     * \param downloadRate New download rate.
     * \param uploadRate   New upload rate.
     */
    bool SetConnectionRates(int downloadRate, int uploadRate);

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
     * \param player       Player object to which the message should be send.
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
     * \param variant     Notice variant (0 = normal, 1 = sad, 2 = happy).
     */
    bool SendNotice(std::string text, std::string avatarLogin = "", int variant = 0);

    /*!
     * \brief Send notice to player login.
     *
     * \param login       Player login.
     * \param text        Chat message.
     * \param avatarLogin Avatar login.
     * \param variant     Notice variant (0 = normal, 1 = sad, 2 = happy).
     */
    bool SendNoticeToLogin(std::string login, std::string text, std::string avatarLogin = "", int variant = 0);

    /*!
     * \brief Send link to player login.
     *
     * \param login       Player login.
     * \param link        Link address.
     * \param linkType    Link type (0 = external, 1 = ManiaLink).
     */
    bool SendOpenLinkToLogin(std::string login, std::string link, int linkType);

    /*!
     * \brief Start callvote.
     *
     * \param xmlRequest  Callvote request.
     */
    bool CallVote(std::string xmlRequest);

    /*!
     * \brief Start callvote.
     *
     * \param xmlRequest  Callvote request.
     * \param ratio       Passing ratio (between 0 and 1, -1 = default).
     * \param timeout     Timeout for the callvote (0 = default, 1 = indefinite).
     * \param whoVotes    Who votes for the callvote (0 = active players, 1 = all players, 2 = everyone, including spectators).
     */
    bool CallVoteEx(std::string xmlRequest, double ratio, int timeout, int whoVotes);

    /*!
     * \brief Cancels the current callvote.
     */
    bool CancelVote();

    /*!
     * \brief Returns the current callvote.
     */
    CurrentCallVote GetCurrentCallVote();

    /*!
     * \brief Sets the default callvote timeout.
     *
     * \param timeout     Default timeout.
     */
    bool SetCallVoteTimeOut(int timeout);

    /*!
     * \brief Gets the default callvote timeout.
     */
    CurrentNextValue GetCallVoteTimeOut();

    /*!
     * \brief Sets the default callvote ratio.
     *
     * \param ratio       Default timeout.
     */
    bool SetCallVoteRatio(double ratio);

    /*!
     * \brief Gets the default callvote ratio.
     */
    double GetCallVoteRatio();

    /*!
     * \brief Sets the callvote ratios.
     *
     * \param ratios      List of callvote ratios.
     */
    bool SetCallVoteRatios(std::vector<CallVoteRatio> ratios);

    /*!
     * \brief Gets the callvote ratios.
     */
    std::vector<CallVoteRatio> GetCallVoteRatios();

    /*!
     * \brief Sets the callvote ratios.
     *
     * \param replaceAll  Replace all current callvote ratios?
     * \param ratios      List of callvote ratios.
     *
     * \todo Implement when GbxParameters supports structs.
     */
    bool SetCallVoteRatiosEx(bool replaceAll, std::vector<ExtendedCallVoteRatio> ratios);

    /*!
     * \brief Gets the callvote ratios.
     */
    std::vector<ExtendedCallVoteRatio> GetCallVoteRatiosEx();

    /*!
     * \brief Displays ManiaLink page to all players.
     *
     * \param page        XML page to be displayed.
     * \param timeout     Timeout to autohide (0 = permanent).
     * \param hideOnClick Hide the page if a page option is clicked.
     */
    bool SendDisplayManialinkPage(std::string page, int timeout, bool hideOnClick);

    /*!
     * \brief Displays ManiaLink page to a player.
     *
     * \param login       Player login.
     * \param page        XML page to be displayed.
     * \param timeout     Timeout to autohide (0 = permanent).
     * \param hideOnClick Hide the page if a page option is clicked.
     */
    bool SendDisplayManialinkPageToLogin(std::string login, std::string page, int timeout, bool hideOnClick);

    /*!
     * \brief Hides all ManiaLink pages for all players.
     */
    bool SendHideManialinkPage();

    /*!
     * \brief Hides all ManiaLink pages for a player.
     *
     * \param login       Player login.
     */
    bool SendHideManialinkPageToLogin(std::string login);

    /*!
     * \brief Returns the latest results from the current ManiaLink page.
     */
    std::vector<ManiaLinkPageAnswer> GetManialinkPageAnswers();

    /*bool SendOpenLinkToLogin(std::string login, std::string link, int linkType);

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
    std::string GetSkinsDirectory();*/

    /*!
     * \brief Provides a map object for the map being played next on the server.
     */
    Map GetNextMapInfo();

    /*!
     * \brief Provides a map object for the map currently being played on the server.
     *
     * This is mainly meant for the start-up of the controller.
     * The current map can always be request via the Current property of MapList.
     */
    Map GetCurrentMapInfo();

    /*!
     * \brief Provides a map object for the map with the specified file name.
     *
     * \param fileName  File name of the map.
     */
    Map GetMapInfo(std::string fileName);

    /*!
     * \brief Checks if the map matches the current server settings.
     *
     * \param fileName  File name of the map.
     */
    bool CheckMapForCurrentServerParams(std::string fileName);

    /*!
     * \brief Gets the maplist from the server.
     *
     * \param limit     Maximum amount of maps to be retrieved.
     * \param index     Map index on which to start the retrieval.
     */
    std::vector<Map> GetMapList(int limit, int index);

    /*!
     * \brief Add the map with the specified filename at the end of the current selection.
     *
     * \param fileName  Filename of the map.
     */
    bool AddMap(std::string fileName);

    /*!
     * \brief Add the list of maps with the specified filenames at the end of the current selection.
     *
     * \param fileNames List of map filenames.
     */
    int AddMapList(std::vector<std::string> fileNames);

    /*!
     * \brief Remove the map with the specified filename from the current selection.
     *
     * \param fileName  Filename of the map.
     */
    bool RemoveMap(std::string fileName);

    /*!
     * \brief Remove the list of maps with the specified filenames from the current selection.
     *
     * \param fileNames List of map filenames.
     */
    int RemoveMapList(std::vector<std::string> fileNames);

    /*!
     * \brief Insert the map with the specified filename after the current map.
     *
     * \param fileName  Filename of the map.
     */
    bool InsertMap(std::string fileName);

    /*!
     * \brief Insert the list of maps with the specified filenames after the current map.
     *
     * \param fileNames List of map filenames.
     */
    int InsertMapList(std::vector<std::string> fileNames);

    /*!
     * \brief Set as next map the one with the specified filename, if it is present in the selection.
     *
     * \param fileName  Filename of the map.
     */
    bool ChooseNextMap(std::string fileName);

    /*!
     * \brief Set as next maps the list of maps with the specified filenames, if they are present in the selection.
     *
     * \param fileNames List of map filenames.
     */
    int ChooseNextMapList(std::vector<std::string> fileNames);

private:
    GbxRemote* server;                         /**< \brief Pointer to GbxRemote. */
    std::map<std::string, Player>* playerList; /**< \brief Pointer to the playerlist. */
};

#endif // METHODS_H_
