#include "CallBackManager.h"

CallBackManager::CallBackManager(GbxRemote* serverPtr, CommandManager* commandManagerPtr, EventManager* eventManagerPtr, sql::Connection* databasePtr, std::map<std::string, Player>* playerList, MapList* mapList)
{
    server = serverPtr;
    commands = commandManagerPtr;
    events = eventManagerPtr;
    database = databasePtr;

    players = playerList;
    maps = mapList;
}

void CallBackManager::HandleCallBack(std::string methodName, std::vector<GbxResponseParameter> parameters)
{
    std::cout << "CALLBACK: " << methodName << " (parameters: " << parameters.size() << ")" << std::endl;
    /*for(int paramId = 0; paramId < parameters.size(); paramId++)
    {
        GbxResponseParameter parameter = parameters.at(paramId);
        Logging::PrintParameter(parameter, paramId);
    }*/

    if(methodName.find("ManiaPlanet.PlayerConnect") != std::string::npos)
    {
        HandlePlayerConnect(parameters);
    }
    else if(methodName.find("ManiaPlanet.PlayerDisconnect") != std::string::npos)
    {
        HandlePlayerDisconnect(parameters);
    }
    else if(methodName.find("ManiaPlanet.PlayerChat") != std::string::npos)
    {
        HandlePlayerChat(parameters);
    }
    else if(methodName.find("ManiaPlanet.PlayerManialinkPageAnswer") != std::string::npos)
    {
        HandlePlayerManialinkPageAnswer(parameters);
    }
    else if(methodName.find("ManiaPlanet.Echo") != std::string::npos)
    {
        HandleEcho(parameters);
    }
    else if(methodName.find("ManiaPlanet.BeginMatch") != std::string::npos)
    {
        HandleBeginMatch(parameters);
    }
    else if(methodName.find("ManiaPlanet.EndMatch") != std::string::npos)
    {
        HandleEndMatch(parameters);
    }
    else if(methodName.find("ManiaPlanet.BeginMap") != std::string::npos)
    {
        HandleBeginMap(parameters);
    }
    else if(methodName.find("ManiaPlanet.EndMap") != std::string::npos)
    {
        HandleEndMap(parameters);
    }
    else if(methodName.find("ManiaPlanet.StatusChanged") != std::string::npos)
    {
        HandleStatusChanged(parameters);
    }
    else if(methodName.find("TrackMania.PlayerCheckpoint") != std::string::npos)
    {
        HandlePlayerCheckpoint(parameters);
    }
    else if(methodName.find("TrackMania.PlayerFinish") != std::string::npos)
    {
        HandlePlayerFinish(parameters);
    }
    else if(methodName.find("TrackMania.PlayerIncoherence") != std::string::npos)
    {
        HandlePlayerIncoherence(parameters);
    }
    else if(methodName.find("ManiaPlanet.BillUpdated") != std::string::npos)
    {
        HandleBillUpdated(parameters);
    }
    else if(methodName.find("ManiaPlanet.MapListModified") != std::string::npos)
    {
        HandleMapListModified(parameters);
    }
    else if(methodName.find("ManiaPlanet.PlayerInfoChanged") != std::string::npos)
    {
        HandlePlayerInfoChanged(parameters);
    }
    else if(methodName.find("ManiaPlanet.VoteUpdated") != std::string::npos)
    {
        HandleVoteUpdated(parameters);
    }
}

void CallBackManager::HandlePlayerConnect(std::vector<GbxResponseParameter> parameters)
{
    GbxParameters params = GbxParameters();
    std::string login = parameters.at(0).GetString();
    params.Put(&login);

    GbxMessage message = GbxMessage("GetPlayerInfo", params);
    server->Query(message);
    Player newPlayer = Player(server->GetResponse()->GetParameters().at(0).GetStruct());
    
    message = GbxMessage("GetDetailedPlayerInfo", params);
    server->Query(message);
    newPlayer.PlayerDetailed(server->GetResponse()->GetParameters().at(0).GetStruct());

    if(database != NULL)
    {
        sql::PreparedStatement* pstmt;
        pstmt = database->prepareStatement("SELECT * FROM `players` WHERE `Login` = ?");
        pstmt->setString(1, newPlayer.Login);
        sql::ResultSet* result = pstmt->executeQuery();
        if(result->next())
        {
            newPlayer.SetId(result->getInt("Id"));
        }
    }

    players->insert(std::pair<std::string, Player>(newPlayer.Login, newPlayer));

    std::cout << "Player connected: " << newPlayer.Login << " (# players: " << players->size() << ")" << std::endl;

    events->CallPlayerConnect(newPlayer);
}

void CallBackManager::HandlePlayerDisconnect(std::vector<GbxResponseParameter> parameters)
{
    std::string login = parameters.at(0).GetString();

    Player disconnectingPlayer = players->find(login)->second;
    players->erase(disconnectingPlayer.Login);
    std::cout << "Player disconnected: " << disconnectingPlayer.Login << " (# players: " << players->size() << ")" << std::endl;

    events->CallPlayerDisconnect(disconnectingPlayer);
}

void CallBackManager::HandlePlayerChat(std::vector<GbxResponseParameter> parameters)
{
    int playerUid = atoi(parameters.at(0).GetString().c_str());

    if(playerUid != 0)
    {
        std::string login = parameters.at(1).GetString();
        std::string text = parameters.at(2).GetString();
        bool isRegisteredCmd;
        std::istringstream(parameters.at(3).GetString()) >> isRegisteredCmd;

        Player player = players->at(login);
        if(isRegisteredCmd)
        {
            commands->HandleCommand(player, text);
        }
        else
        {
            events->CallPlayerChat(player, text, isRegisteredCmd);
        }
    }
}

void CallBackManager::HandlePlayerManialinkPageAnswer(std::vector<GbxResponseParameter> parameters)
{
    std::string login = parameters.at(1).GetString();
    Player player = players->at(login);
    std::string answer = parameters.at(2).GetString();

    std::vector<GbxResponseParameter> entryParams = parameters.at(3).GetArray();
    std::vector<EntryVal> entries = std::vector<EntryVal>();
    for(int entryId = 0; entryId < entryParams.size(); entryId++)
    {
        EntryVal entry = EntryVal();
        std::map<std::string, GbxResponseParameter> entryVal = entryParams.at(entryId).GetStruct();

        entry.Name = entryVal.at("Name").GetString();
        entry.Value = entryVal.at("Value").GetString();

        entries.push_back(entry);
    }

    events->CallPlayerManialinkPageAnswer(player, answer, entries);
}

void CallBackManager::HandleEcho(std::vector<GbxResponseParameter> parameters)
{
    std::string internal = parameters.at(0).GetString();
    std::string publicEcho = parameters.at(1).GetString();

    events->CallEcho(internal, publicEcho);
}

void CallBackManager::HandleBeginMatch(std::vector<GbxResponseParameter> parameters)
{
    events->CallBeginMatch();
}

void CallBackManager::HandleEndMatch(std::vector<GbxResponseParameter> parameters)
{
    std::vector<GbxResponseParameter> rankingParams = parameters.at(0).GetArray();
    std::vector<PlayerRanking> rankings = std::vector<PlayerRanking>();
    for(int entryId = 0; entryId < rankingParams.size(); entryId++)
    {
        PlayerRanking ranking = PlayerRanking();
        std::map<std::string, GbxResponseParameter> rankingVal = rankingParams.at(entryId).GetStruct();

        std::string login = rankingVal.at("Login").GetString();
        if(players->find(login) != players->end())
        {
            ranking.PlayerInfo = players->at(login);
        }
        else
        {
            ranking.PlayerInfo = Player();
            ranking.PlayerInfo.PlayerId = atoi(rankingVal.at("PlayerId").GetString().c_str());
            ranking.PlayerInfo.Login = rankingVal.at("Login").GetString();
            ranking.PlayerInfo.NickName = rankingVal.at("NickName").GetString();
        }
        ranking.Rank = atoi(rankingVal.at("Rank").GetString().c_str());
        ranking.BestTime = atoi(rankingVal.at("BestTime").GetString().c_str());
        ranking.Score = atoi(rankingVal.at("Score").GetString().c_str());
        ranking.NbrLapsFinished = atoi(rankingVal.at("NbrLapsFinished").GetString().c_str());
        ranking.LadderScore = atof(rankingVal.at("LadderScore").GetString().c_str());

        std::vector<int> checkpoints = std::vector<int>();
        std::vector<GbxResponseParameter> checkpointsParam = rankingVal.at("BestCheckpoints").GetArray();
        for(int checkpointId = 0; checkpointId < checkpointsParam.size(); checkpointId++)
        {
            checkpoints.push_back(atoi(checkpointsParam.at(checkpointId).GetString().c_str()));
        }
        ranking.BestCheckpoints = checkpoints;

        rankings.push_back(ranking);
    }

    int winnerTeam = atoi(parameters.at(1).GetString().c_str());

    events->CallEndMatch(rankings, winnerTeam);
}

void CallBackManager::HandleBeginMap(std::vector<GbxResponseParameter> parameters)
{
    std::map<std::string, GbxResponseParameter> mapStruct = parameters.at(0).GetStruct();
    Map* map = &(maps->List.at(mapStruct.at("UId").GetString()));
    map->MapDetailed(parameters.at(0).GetStruct());

    maps->SetCurrentMap(mapStruct.at("UId").GetString());
    events->CallBeginMap(*map);
}

void CallBackManager::HandleEndMap(std::vector<GbxResponseParameter> parameters)
{
    std::map<std::string, GbxResponseParameter> mapStruct = parameters.at(0).GetStruct();
    Map map = maps->List.at(mapStruct.at("UId").GetString());
    events->CallEndMap(map);
}

void CallBackManager::HandleStatusChanged(std::vector<GbxResponseParameter> parameters)
{
    int statusCode = atoi(parameters.at(0).GetString().c_str());
    std::string statusName = parameters.at(1).GetString();

    events->CallStatusChanged(statusCode, statusName);
}

void CallBackManager::HandlePlayerCheckpoint(std::vector<GbxResponseParameter> parameters)
{
    std::string login = parameters.at(1).GetString();
    Player* player = &players->at(login);
    int time = atoi(parameters.at(2).GetString().c_str());
    player->CurrentCheckpoints.push_back(time);
    int currentLap = atoi(parameters.at(3).GetString().c_str());
    int checkpointIndex = atoi(parameters.at(4).GetString().c_str());

    events->CallPlayerCheckpoint(*player, time, currentLap, checkpointIndex);
}

void CallBackManager::HandlePlayerFinish(std::vector<GbxResponseParameter> parameters)
{
    std::string login = parameters.at(1).GetString();
    Player* player = &players->at(login);
    int time = atoi(parameters.at(2).GetString().c_str());

    events->CallPlayerFinish(*player, time);
    player->CurrentCheckpoints.clear();
}

void CallBackManager::HandlePlayerIncoherence(std::vector<GbxResponseParameter> parameters)
{
    std::string login = parameters.at(0).GetString();
    Player player = players->at(login);

    events->CallPlayerIncoherence(player);
}

void CallBackManager::HandleBillUpdated(std::vector<GbxResponseParameter> parameters)
{
    int billId = atoi(parameters.at(0).GetString().c_str());
    int state = atoi(parameters.at(1).GetString().c_str());
    std::string stateName = parameters.at(2).GetString().c_str();
    int transactionId = atoi(parameters.at(3).GetString().c_str());

    events->CallBillUpdated(billId, state, stateName, transactionId);
}

void CallBackManager::HandleMapListModified(std::vector<GbxResponseParameter> parameters)
{
    int currentMapIndex = atoi(parameters.at(0).GetString().c_str());
    int nextMapIndex = atoi(parameters.at(1).GetString().c_str());
    bool listModified;
    std::istringstream(parameters.at(2).GetString()) >> listModified;

    events->CallMapListModified(currentMapIndex, nextMapIndex, listModified);
}

void CallBackManager::HandlePlayerInfoChanged(std::vector<GbxResponseParameter> parameters)
{
    std::map<std::string, GbxResponseParameter> playerInfo = parameters.at(0).GetStruct();
    Player player = Player(playerInfo);

    events->CallPlayerInfoChanged(player);
}

void CallBackManager::HandleVoteUpdated(std::vector<GbxResponseParameter> parameters)
{
    std::string stateName = parameters.at(0).GetString();
    std::string login = parameters.at(1).GetString();
    std::string commandName = parameters.at(2).GetString();
    std::string commandParam = parameters.at(3).GetString();

    events->CallVoteUpdated(stateName, login, commandName, commandParam);
}
