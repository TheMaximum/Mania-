#include "KarmaPlugin.h"

KarmaPlugin::KarmaPlugin()
{
    Version = "0.2.0";
    Author = "TheM";
    karma = MapKarma();

    BeginMap.push_back([this](Map map) { OnBeginMap(map); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    PlayerChat.push_back([this](Player player, std::string text) { OnPlayerChat(player, text); });

    RegisterCommand("karma", [this](Player player, std::vector<std::string> parameters) { DisplayCurrentKarma(player); });
    RegisterCommand("whokarma", [this](Player player, std::vector<std::string> parameters) { DisplayWhoKarma(player); });
    RegisterCommand("++", [this](Player player, std::vector<std::string> parameters) { VotePositive(player); });
    RegisterCommand("--", [this](Player player, std::vector<std::string> parameters) { VoteNegative(player); });
    RegisterCallableMethod("GetKarmaByMapId", [this](boost::any parameters) { return GetKarmaByMapId(parameters); });
}

void KarmaPlugin::Init()
{
    loadSettings();
    widget = KarmaWidget(controller->UI);
    widget.WidgetX = widgetX;
    widget.WidgetY = widgetY;
    controller->UI->RegisterEvent(widget.ActionId, ([this](Player player, std::string answer, std::vector<EntryVal> entries) { DisplayWhoKarma(player); }));
    controller->UI->RegisterEvent(widget.PositiveAction, ([this](Player player, std::string answer, std::vector<EntryVal> entries) { VotePositive(player); }));
    controller->UI->RegisterEvent(widget.NegativeAction, ([this](Player player, std::string answer, std::vector<EntryVal> entries) { VoteNegative(player); }));

    updateAllMapKarma();

    votes = retrieveVotes(*controller->Maps->Current);
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
    votes = retrieveVotes(*controller->Maps->Current);
    karma.Calculate(votes);
    controller->Maps->Current->UpdateAdditional("Karma", karma.Karma);
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

void KarmaPlugin::DisplayCurrentKarma(Player player)
{
    std::stringstream chatMessage;
    chatMessage << "$ff0Current map karma: $fff" << karma.Karma << "$ff0 [";
    chatMessage << "$fff" << (karma.PlusVotes + karma.MinVotes) << "$ff0 votes,";
    chatMessage << " ++: $fff" << karma.PlusVotes << "$ff0 ($fff" << karma.Percentage << "%$ff0),";
    chatMessage << " --: $fff" << karma.MinVotes << "$ff0 ($fff" << karma.MinPercentage << "%$ff0)]";

    int personalVote = -1;
    std::map<std::string, int>::iterator voteIt = votes.find(player.Login);
    if(voteIt != votes.end())
        personalVote = voteIt->second;

    chatMessage << " {Your vote: $fff";
    switch(personalVote)
    {
        case 1:
            chatMessage << "++";
            break;
        case 0:
            chatMessage << "--";
            break;
        default:
            chatMessage << "none";
            break;
    }
    chatMessage << "$ff0}";

    controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
}

void KarmaPlugin::OnPlayerChat(Player player, std::string text)
{
    if(text.find("++") == 0)
    {
        VotePositive(player);
    }
    else if(text.find("--") == 0)
    {
        VoteNegative(player);
    }
}

void KarmaPlugin::VotePositive(Player player)
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
        sql::PreparedStatement* pstmt;
        try
        {
            pstmt = controller->Database->prepareStatement("INSERT INTO `karma` (`MapId`, `PlayerId`, `Score`) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE `Score` = VALUES(`Score`)");
            pstmt->setInt(1, controller->Maps->Current->Id);
            pstmt->setInt(2, player.Id);
            pstmt->setInt(3, 1);
            pstmt->executeQuery();

            votes = retrieveVotes(*controller->Maps->Current);
            karma.Calculate(votes);
            controller->Maps->Current->UpdateAdditional("Karma", karma.Karma);
            displayToAll();

            if(personalVote == -1)
                chatMessage << "Successfully voted $fff++$ff0 on this map!";
            else
                chatMessage << "Changed your $fff--$ff0 vote to a $fff++$ff0 vote on this map!";
        }
        catch(sql::SQLException &e)
        {
            std::cout << "Failed to save karma for " << player.Login << " on '" << controller->Maps->Current->Name << "' ..." << std::endl;
            Logging::PrintError(e.getErrorCode(), e.what());
        }

        if(pstmt != NULL)
        {
            delete pstmt;
            pstmt = NULL;
        }
    }
    else
    {
        chatMessage << "You already voted $fff++$ff0 on this map!";
    }

    controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
}

void KarmaPlugin::VoteNegative(Player player)
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
        sql::PreparedStatement* pstmt;
        try
        {
            pstmt = controller->Database->prepareStatement("INSERT INTO `karma` (`MapId`, `PlayerId`, `Score`) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE `Score` = VALUES(`Score`)");
            pstmt->setInt(1, controller->Maps->Current->Id);
            pstmt->setInt(2, player.Id);
            pstmt->setInt(3, 0);
            pstmt->executeQuery();

            votes = retrieveVotes(*controller->Maps->Current);
            karma.Calculate(votes);
            controller->Maps->Current->UpdateAdditional("Karma", karma.Karma);
            displayToAll();

            if(personalVote == -1)
                chatMessage << "Successfully voted $fff--$ff0 on this map!";
            else
                chatMessage << "Changed your $fff++$ff0 vote to a $fff--$ff0 vote on this map!";
        }
        catch(sql::SQLException &e)
        {
            std::cout << "Failed to save karma for " << player.Login << " on '" << controller->Maps->Current->Name << "' ..." << std::endl;
            Logging::PrintError(e.getErrorCode(), e.what());
        }

        if(pstmt != NULL)
        {
            delete pstmt;
            pstmt = NULL;
        }
    }
    else
    {
        chatMessage << "You already voted $fff--$ff0 on this map!";
    }

    controller->Server->ChatSendServerMessageToLogin(chatMessage.str(), player.Login);
}

void KarmaPlugin::DisplayWhoKarma(Player player)
{
    std::vector<std::pair<std::string, int>> votesVector;
    std::copy(votes.begin(), votes.end(), back_inserter(votesVector));
    std::sort(votesVector.begin(), votesVector.end(), [=](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { return a.second > b.second; });

    UIList list = UIList();
    list.Id = "WhoKarma";
    list.Title = "WhoKarma for: $z$s$fff" + controller->Maps->Current->Name;
    list.IconStyle = "Icons128x128_1";
    list.IconSubstyle = "CustomStars";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Player", 40));
    list.Columns.push_back(std::pair<std::string, int>("Vote", 20));

    for(int voteId = 0; voteId < votesVector.size(); voteId++)
    {
        std::string vote = "++";
        if(votesVector[voteId].second == -1)
            vote = "--";

        std::string playerName = votesVector[voteId].first;
        sql::PreparedStatement* pstmt;
        sql::ResultSet* result;
        try
        {
            pstmt = controller->Database->prepareStatement("SELECT * FROM `players` WHERE `Login` = ?");
            pstmt->setString(1, playerName);
            result = pstmt->executeQuery();
            if(result->next())
            {
                playerName = result->getString("NickName");
            }
        }
        catch(sql::SQLException &e) { }

        if(pstmt != NULL)
            delete pstmt; pstmt = NULL;

        if(result != NULL)
            delete result; result = NULL;

        std::stringstream index;
        index << (voteId + 1);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", index.str()));
        row.insert(std::pair<std::string, std::string>("Player", playerName));
        row.insert(std::pair<std::string, std::string>("Vote", vote));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

boost::any KarmaPlugin::GetKarmaByMapId(boost::any parameters)
{
    boost::any result = 0;
    int mapId = boost::any_cast<int>(parameters);
    if(mapId != 0)
    {
        sql::PreparedStatement* pstmt;
        sql::ResultSet* dbResult;
        try
        {
            pstmt = controller->Database->prepareStatement("SELECT SUM(Score) as Karma FROM `karma` WHERE `MapId` = ?");
            pstmt->setInt(1, mapId);
            dbResult = pstmt->executeQuery();
            dbResult->next();
            result = dbResult->getInt("Karma");
        }
        catch(sql::SQLException &e)
        {
            
        }

        if(pstmt != NULL)
        {
            delete pstmt;
            pstmt = NULL;
        }

        if(dbResult != NULL)
        {
            delete dbResult;
            dbResult = NULL;
        }
    }

    return result;
}

void KarmaPlugin::updateAllMapKarma()
{
    std::cout << "[          ] Retrieving karma for all maps ... " << '\r' << std::flush;
    for(std::map<std::string, Map>::iterator mapIt = controller->Maps->List.begin(); mapIt != controller->Maps->List.end(); ++mapIt)
    {
        mapIt->second.UpdateAdditional("Karma", GetKarmaByMapId(mapIt->second.Id));
    }
    Logging::PrintOKFlush();
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

std::map<std::string, int> KarmaPlugin::retrieveVotes(Map map)
{
    std::map<std::string, int> mapVotes = std::map<std::string, int>();

    sql::PreparedStatement* pstmt;
    sql::ResultSet* result;
    try
    {
        pstmt = controller->Database->prepareStatement("SELECT * FROM `karma` WHERE `MapId` = ?");
        pstmt->setInt(1, map.Id);
        result = pstmt->executeQuery();

        while(result->next())
        {
            delete pstmt; pstmt = NULL;
            sql::ResultSet* playerResult;
            try
            {
                pstmt = controller->Database->prepareStatement("SELECT * FROM `players` WHERE `Id` = ?");
                pstmt->setInt(1, result->getInt("PlayerId"));
                playerResult = pstmt->executeQuery();
                playerResult->next();

                std::string login = playerResult->getString("Login");
                int score = result->getInt("Score");

                mapVotes.insert(std::pair<std::string, int>(login, score));
            }
            catch(sql::InvalidArgumentException &e)
            {
                // Player cannot be found in the database.
                // Just skip the karma vote.
            }

            if(playerResult != NULL)
            {
                delete playerResult;
                playerResult = NULL;
            }
        }
    }
    catch(sql::SQLException &e)
    {
        std::cout << "Failed to retrieve karma votes for '" << map.Name << "' ..." << std::endl;
        Logging::PrintError(e.getErrorCode(), e.what());
    }

    if(pstmt != NULL)
    {
        delete pstmt;
        pstmt = NULL;
    }

    if(result != NULL)
    {
        delete result;
        result = NULL;
    }

    return mapVotes;
}

int KarmaPlugin::retrieveTimesDriven(Player player)
{
    int timesDriven = 0;

    sql::PreparedStatement* pstmt;
    sql::ResultSet* result;
    try
    {
        pstmt = controller->Database->prepareStatement("SELECT COUNT(*) AS `timesDriven` FROM `times` WHERE `PlayerId` = ? AND `MapId` = ?");
        pstmt->setInt(1, player.Id);
        pstmt->setInt(2, controller->Maps->Current->Id);
        result = pstmt->executeQuery();
        result->next();

        timesDriven = result->getInt("timesDriven");
    }
    catch(sql::SQLException &e)
    {
        std::cout << "Failed to retrieve # times driven for player " << player.Login << " on map '" << controller->Maps->Current->Name << "' ..." << std::endl;
        Logging::PrintError(e.getErrorCode(), e.what());
    }

    if(pstmt != NULL)
    {
        delete pstmt;
        pstmt = NULL;
    }

    if(result != NULL)
    {
        delete result;
        result = NULL;
    }

    return timesDriven;
}
