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
    void ChatUnban(Player player, std::vector<std::string> parameters);
    void ChatBlackList(Player player, std::vector<std::string> parameters);
    void ChatUnBlackList(Player player, std::vector<std::string> parameters);
};

#endif // ADMINPLAYERSPLUGIN_H_
