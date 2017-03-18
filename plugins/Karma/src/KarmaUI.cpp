#include "KarmaPlugin.h"

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
