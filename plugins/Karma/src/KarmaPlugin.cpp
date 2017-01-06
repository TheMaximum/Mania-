#include "KarmaPlugin.h"

KarmaPlugin::KarmaPlugin()
{
    Version = "0.1.0";
    Author = "TheM";
    karma = MapKarma();

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    PlayerChat.push_back([this](Player player, std::string text) { OnPlayerChat(player, text); });

    RegisterCommand("++", [this](Player player, std::vector<std::string> parameters) { VotePositiveChat(player, parameters); });
    RegisterCommand("--", [this](Player player, std::vector<std::string> parameters) { VotePositiveChat(player, parameters); });
}

void KarmaPlugin::Init()
{
    loadSettings();
    widget = KarmaWidget(controller->UI);
    widget.WidgetX = widgetX;
    widget.WidgetY = widgetY;
    controller->UI->AddEvent(widget.PositiveAction, ([this](Player player, std::string answer, std::vector<EntryVal> entries) { VotePositiveAction(player, answer, entries); }));
    controller->UI->AddEvent(widget.NegativeAction, ([this](Player player, std::string answer, std::vector<EntryVal> entries) { VoteNegativeAction(player, answer, entries); }));

    retrieveVotes(*controller->Maps->Current);
    karma.Calculate(votes);
    displayToAll();
}

void KarmaPlugin::loadSettings()
{
    std::map<std::string, std::string>::iterator widgetXIt = Settings.find("widgetX");
    if(widgetXIt != Settings.end())
        widgetX = atof(widgetXIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetYIt = Settings.find("widgetY");
    if(widgetYIt != Settings.end())
        widgetY = atof(widgetYIt->second.c_str());

    std::map<std::string, std::string>::iterator voteAfterFinishesIt = Settings.find("voteAfterFinishes");
    if(voteAfterFinishesIt != Settings.end())
        voteAfterFinishes = atoi(voteAfterFinishesIt->second.c_str());
}

void KarmaPlugin::OnBeginMap(Map map)
{
    retrieveVotes(*controller->Maps->Current);
    karma.Calculate(votes);
    displayToAll();
}

void KarmaPlugin::OnPlayerConnect(Player player)
{
    int personalVote = -1;
    std::map<std::string, int>::iterator voteIt = votes.find(player.Login);
    if(voteIt != votes.end())
        personalVote = voteIt->second;

    if(!widget.DisplayToPlayer(player, &karma, personalVote))
        Logging::PrintError(controller->Server->GetCurrentError());
}

void KarmaPlugin::OnPlayerChat(Player player, std::string text)
{
    if(text.find("++") == 0)
    {
        votePositive(player);
    }
    else if(text.find("--") == 0)
    {
        voteNegative(player);
    }
}

void KarmaPlugin::VotePositiveAction(Player player, std::string answer, std::vector<EntryVal> entries)
{
    votePositive(player);
}

void KarmaPlugin::VoteNegativeAction(Player player, std::string answer, std::vector<EntryVal> entries)
{
    voteNegative(player);
}

void KarmaPlugin::VotePositiveChat(Player player, std::vector<std::string> parameters)
{
    votePositive(player);
}

void KarmaPlugin::VoteNegativeChat(Player player, std::vector<std::string> parameters)
{
    voteNegative(player);
}

void KarmaPlugin::votePositive(Player player)
{
    std::stringstream chatMessage;
    chatMessage << "$ff0";

    if(voteAfterFinishes > 0)
    {
        int timesDriven = retrieveTimesDriven(player);
        if(timesDriven < voteAfterFinishes)
        {
            chatMessage << "You can only vote after $fff" << voteAfterFinishes << "$ff0 finishes (you have: $fff" << timesDriven << "$ff0).";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
            return;
        }
    }

    int personalVote = -1;
    std::map<std::string, int>::iterator voteIt = votes.find(player.Login);
    if(voteIt != votes.end())
        personalVote = voteIt->second;

    if(personalVote != 1)
    {
        sql::PreparedStatement* pstmt = controller->Database->prepareStatement("INSERT INTO `rs_karma` (`MapId`, `PlayerId`, `Score`) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE `Score` = VALUES(`Score`)");
        pstmt->setInt(1, controller->Maps->Current->Id);
        pstmt->setInt(2, player.Id);
        pstmt->setInt(3, 1);
        pstmt->executeQuery();
        delete pstmt; pstmt = NULL;

        retrieveVotes(*controller->Maps->Current);
        karma.Calculate(votes);
        displayToAll();

        if(personalVote == -1)
            chatMessage << "Successfully voted $fff++$ff0 on this map!";
        else
            chatMessage << "Changed your $fff--$ff0 vote to a $fff++$ff0 vote on this map!";
    }
    else
    {
        chatMessage << "You already voted $fff++$ff0 on this map!";
    }

    controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
}

void KarmaPlugin::voteNegative(Player player)
{
    std::stringstream chatMessage;
    chatMessage << "$ff0";

    if(voteAfterFinishes > 0)
    {
        int timesDriven = retrieveTimesDriven(player);
        if(timesDriven < voteAfterFinishes)
        {
            chatMessage << "You can only vote after $fff" << voteAfterFinishes << "$ff0 finishes (you have: $fff" << timesDriven << "$ff0).";
            controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
            return;
        }
    }

    int personalVote = -1;
    std::map<std::string, int>::iterator voteIt = votes.find(player.Login);
    if(voteIt != votes.end())
        personalVote = voteIt->second;

    if(personalVote != 0)
    {
        sql::PreparedStatement* pstmt = controller->Database->prepareStatement("INSERT INTO `rs_karma` (`MapId`, `PlayerId`, `Score`) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE `Score` = VALUES(`Score`)");
        pstmt->setInt(1, controller->Maps->Current->Id);
        pstmt->setInt(2, player.Id);
        pstmt->setInt(3, 0);
        pstmt->executeQuery();
        delete pstmt; pstmt = NULL;

        retrieveVotes(*controller->Maps->Current);
        karma.Calculate(votes);
        displayToAll();

        if(personalVote == -1)
            chatMessage << "Successfully voted $fff--$ff0 on this map!";
        else
            chatMessage << "Changed your $fff++$ff0 vote to a $fff--$ff0 vote on this map!";
    }
    else
    {
        chatMessage << "You already voted $fff--$ff0 on this map!";
    }

    controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
}

void KarmaPlugin::displayToAll()
{
    for(std::map<std::string, Player>::iterator player = controller->Players->begin(); player != controller->Players->end(); ++player)
    {
        int personalVote = -1;
        std::map<std::string, int>::iterator voteIt = votes.find(player->second.Login);
        if(voteIt != votes.end())
            personalVote = voteIt->second;

        if(!widget.DisplayToPlayer(player->second, &karma, personalVote))
            Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void KarmaPlugin::retrieveVotes(Map map)
{
    votes = std::map<std::string, int>();

    sql::PreparedStatement* pstmt = controller->Database->prepareStatement("SELECT * FROM `rs_karma` WHERE `MapId` = ?");
    pstmt->setInt(1, map.Id);
    sql::ResultSet* result = pstmt->executeQuery();

    while(result->next())
    {
        delete pstmt; pstmt = NULL;

        pstmt = controller->Database->prepareStatement("SELECT * FROM `players` WHERE `Id` = ?");
        pstmt->setInt(1, result->getInt("PlayerId"));
        sql::ResultSet* playerResult = pstmt->executeQuery();
        playerResult->next();

        std::string login = playerResult->getString("Login");
        int score = result->getInt("Score");

        votes.insert(std::pair<std::string, int>(login, score));

        delete playerResult; playerResult = NULL;
    }

    if(pstmt != NULL)
        delete pstmt; pstmt = NULL;
    delete result; result = NULL;
}

int KarmaPlugin::retrieveTimesDriven(Player player)
{
    votes = std::map<std::string, int>();

    sql::PreparedStatement* pstmt = controller->Database->prepareStatement("SELECT COUNT(*) AS `timesDriven` FROM `rs_times` WHERE `PlayerId` = ? AND `MapId` = ?");
    pstmt->setInt(1, player.Id);
    pstmt->setInt(2, controller->Maps->Current->Id);
    sql::ResultSet* result = pstmt->executeQuery();
    result->next();

    int timesDriven = result->getInt("timesDriven");

    delete pstmt; pstmt = NULL;
    delete result; result = NULL;

    return timesDriven;
}
