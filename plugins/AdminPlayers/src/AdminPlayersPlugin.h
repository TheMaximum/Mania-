#ifndef ADMINPLAYERSPLUGIN_H_
#define ADMINPLAYERSPLUGIN_H_

#include <sstream>
#include "Plugins/Plugin.h"

class AdminPlayersPlugin : public Plugin
{
public:
    AdminPlayersPlugin();

    void Init();

    void ChatKick(Player player, std::vector<std::string> parameters);

    void ChatBan(Player player, std::vector<std::string> parameters);
    void ChatUnBan(Player player, std::vector<std::string> parameters);
    void ChatShowBanList(Player player);
    void ChatClearBanList(Player player);

    void ChatBlackList(Player player, std::vector<std::string> parameters);
    void ChatUnBlackList(Player player, std::vector<std::string> parameters);
    void ChatShowBlackList(Player player);
    void ChatClearBlackList(Player player);
    void ChatLoadBlackList(Player player, std::vector<std::string> parameters);
    void ChatSaveBlackList(Player player, std::vector<std::string> parameters);

    void ChatGuest(Player player, std::vector<std::string> parameters);
    void ChatUnGuest(Player player, std::vector<std::string> parameters);
    void ChatShowGuestList(Player player);
    void ChatClearGuestList(Player player);
    void ChatLoadGuestList(Player player, std::vector<std::string> parameters);
    void ChatSaveGuestList(Player player, std::vector<std::string> parameters);

    void ChatIgnore(Player player, std::vector<std::string> parameters);
    void ChatUnIgnore(Player player, std::vector<std::string> parameters);
    void ChatShowIgnoreList(Player player);
    void ChatClearIgnoreList(Player player);
};

#endif // ADMINPLAYERSPLUGIN_H_
