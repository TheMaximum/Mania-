#include "UIManager.h"

UIManager::UIManager(Methods* serverPtr, EventManager* eventsPtr, std::map<std::string, Player>* playersPtr)
{
    server = serverPtr;
    events = eventsPtr;
    players = playersPtr;
    answers = std::map<std::string, std::function<void(Player, std::string, std::vector<EntryVal>)>>();
    currentLists = std::map<std::string, UIList>();

    std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>> functions = std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>>();
    functions.push_back([this](Player player, std::string answer, std::vector<EntryVal> entries) { OnPlayerManialinkPageAnswer(player, answer, entries); });
    events->RegisterPlayerManialinkPageAnswer(functions);

    std::vector<std::function<void(std::vector<PlayerRanking>, int)>> functionsEndMatch = std::vector<std::function<void(std::vector<PlayerRanking>, int)>>();
    functionsEndMatch.push_back([this](std::vector<PlayerRanking> rankings, int winnerTeam) { OnEndMatch(); });
    events->RegisterEndMatch(functionsEndMatch);

    std::vector<std::function<void(Player)>> functionsPlayerDisconnect = std::vector<std::function<void(Player)>>();
    functionsPlayerDisconnect.push_back([this](Player player) { CloseList(player); });
    events->RegisterPlayerDisconnect(functionsPlayerDisconnect);

    RegisterEvent("CloseCenterList", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { CloseList(player); }));
}

void UIManager::OnPlayerManialinkPageAnswer(Player player, std::string answer, std::vector<EntryVal> entries)
{
    if(answer.find("PageCenterList(") != std::string::npos)
    {
        std::map<std::string, UIList>::iterator playerUIList = currentLists.find(player.Login);
        if(playerUIList != currentLists.end())
        {
            answer.erase(0, strlen("PageCenterList("));
            std::string answerPage = answer.substr(0, (answer.size() - 1));
            int page = atoi(answerPage.c_str());
            DisplayList(playerUIList->second, player, page);
        }
    }
    else
    {
        if(answers.find(answer) != answers.end())
        {
            std::function<void(Player, std::string, std::vector<EntryVal>)> method = answers.find(answer)->second;
            method(player, answer, entries);
        }
        else
        {
            size_t positionParam = answer.find('(');
            if(positionParam != std::string::npos)
            {
                std::string answerWithoutParam = answer.substr(0, positionParam);
                if(answers.find(answerWithoutParam) != answers.end())
                {
                    std::function<void(Player, std::string, std::vector<EntryVal>)> method = answers.find(answerWithoutParam)->second;
                    method(player, answer, entries);
                }
            }
        }
    }
}

bool UIManager::Display(UIFrame frame)
{
    return server->SendDisplayManialinkPage(formatPage(frame), frame.Timeout, frame.CloseOnClick);
}

bool UIManager::Display(UIFrame frame, Player player)
{
    return server->SendDisplayManialinkPageToLogin(player.Login, formatPage(frame), frame.Timeout, frame.CloseOnClick);
}

bool UIManager::DisplayList(UIList list, Player player, int currentPage)
{
    std::map<std::string, UIList>::iterator playerUIList = currentLists.find(player.Login);
    if(playerUIList == currentLists.end())
    {
        currentLists.insert(std::pair<std::string, UIList>(player.Login, list));
    }
    else if(playerUIList->second.Id != list.Id)
    {
        currentLists.erase(player.Login);
        currentLists.insert(std::pair<std::string, UIList>(player.Login, list));
    }

    UIFrame frame = UIFrame();
    frame.ManiaLinkId = "CenterList";
    frame.Timeout = 0;
    frame.CloseOnClick = false;

    int pages = std::ceil((list.Rows.size() / 20.0));
    if(pages == 0) pages = 1;

    int totalSize = 0;
    for(int columnId = 0; columnId < list.Columns.size(); columnId++)
    {
        totalSize += list.Columns.at(columnId).second;
    }

    if(totalSize > 80)
    {
        std::vector<std::pair<std::string, int>> updatedColumns;
        for(int columnId = 0; columnId < list.Columns.size(); columnId++)
        {
            std::pair<std::string, int> column = list.Columns.at(columnId);
            updatedColumns.push_back(std::pair<std::string, int>(column.first, std::round((column.second / (double)totalSize) * 80.0)));
        }

        list.Columns = updatedColumns;
    }

    std::stringstream widget;
    widget << "<frame posn=\"-42 38 0\" id=\"List" << list.Id << "\">";
    widget << "        <quad posn=\"0 0 0.002\" sizen=\"84 63.5\" style=\"Bgs1InRace\" substyle=\"BgCardList\"/>";
    widget << "        <quad posn=\"0.5 -0.5 0.003\" sizen=\"83 5\" style=\"Bgs1InRace\" substyle=\"BgCardList\"/>";
    widget << "        <quad posn=\"78.5 -0.5 0.004\" sizen=\"5 5\" style=\"Icons64x64_1\" substyle=\"Close\" action=\"CloseCenterList\"/>";
    widget << "        <quad posn=\"1 -0.8 0.004\" sizen=\"4 4\" style=\"" << list.IconStyle << "\" substyle=\"" << list.IconSubstyle << "\"/>";

    widget << "        <label posn=\"5.5 -2 0.004\" sizen=\"55 3\" halign=\"left\" textsize=\"2.5\" text=\"$o" << list.Title << "\"/>";
    widget << "        <label posn=\"78.5 -2.3 0.004\" sizen=\"55 3\" halign=\"right\" textsize=\"1.5\" text=\"$o(" << list.Rows.size() << " entries)\"/>";

    widget << "        <frame posn=\"0.5 -6 0\" id=\"ListWhoKarmaList\">";
    widget << "                <quad posn=\"0 0 0.005\" sizen=\"83 53\" style=\"Bgs1InRace\" substyle=\"BgCardList\"/>";
    widget << "                <quad posn=\"0 0 0.006\" sizen=\"83 3\" style=\"Bgs1InRace\" substyle=\"BgCardList\"/>";

    int columnX = 1;
    for(int columnId = 0; columnId < list.Columns.size(); columnId++)
    {
        std::pair<std::string, int> column = list.Columns.at(columnId);
        widget << "                        <label posn=\"" << columnX << " -0.7 0.007\" sizen=\"" << column.second << " 1.5\" halign=\"left\" textsize=\"1.8\" text=\"$o" << Text::EscapeXML(column.first) << "\"/>";
        columnX += column.second;
    }

    double entryY = -3.5;
    int entry = 1;
    int startRow = ((currentPage - 1) * 20);

    for(int rowId = startRow; rowId < list.Rows.size(); rowId++)
    {
        std::map<std::string, std::string> currentRow = list.Rows.at(rowId);

        if(entry % 2 == 0)
            widget << "                <quad posn=\"0.05 " << (entryY + 0.55) << " 0.006\" sizen=\"82.9 2.6\" bgcolor=\"0003\"/>";

        int rowColumnX = 1;
        for(int columnId = 0; columnId < list.Columns.size(); columnId++)
        {
            std::pair<std::string, int> column = list.Columns.at(columnId);
            std::map<std::string, std::string>::iterator columnRowIt = currentRow.find(column.first);
            std::string labelAction = "";
            if(columnRowIt != currentRow.end())
            {
                std::map<std::string, std::pair<std::string, std::string>>::iterator actionColumnIt = list.Actions.find(column.first);
                if(actionColumnIt != list.Actions.end())
                {
                    std::pair<std::string, std::string> actionInfo = actionColumnIt->second;
                    std::map<std::string, std::string>::iterator actionRowIt = currentRow.find(actionInfo.second);
                    if(actionRowIt != currentRow.end())
                    {
                        labelAction = " action=\"" + actionInfo.first + "(" + actionRowIt->second + ")\" focusareacolor1=\"0000\" focusareacolor2=\"0000\"";
                    }
                }

                widget << "                        <label posn=\"" << rowColumnX << " " << (entryY - 0.65) << " 0.007\" sizen=\"" << (column.second - 0.5) <<" 2.47\" halign=\"left\" valign=\"center\" textsize=\"1.8\"" << labelAction << " text=\" " << Text::EscapeXML(columnRowIt->second) << "\"/>";
            }

            rowColumnX += column.second;
        }

        entryY -= 2.5;
        entry++;

        if(entry > 20) break;
    }

    widget << "        </frame>";

    widget << "        <quad posn=\"0.5 -59.5 0.003\" sizen=\"83 3.5\" style=\"Bgs1InRace\" substyle=\"BgCardList\"/>";

    if(currentPage > 1)
    {
        widget << "        <quad posn=\"0.5 -59.5 0.004\" sizen=\"3.5 3.5\" style=\"Icons64x64_1\" substyle=\"ArrowFirst\" action=\"PageCenterList(1)\"/>";
        widget << "        <quad posn=\"3 -59.5 0.004\" sizen=\"3.5 3.5\" style=\"Icons64x64_1\" substyle=\"ArrowPrev\" action=\"PageCenterList(" << (currentPage - 1) << ")\"/>";
    }

    widget << "        <label posn=\"42 -60.5 0.004\" sizen=\"20 1.5\" halign=\"center\" textsize=\"1.5\" text=\"$oPage " << currentPage << "/" << pages << "\"/>";
    if(currentPage != pages)
    {
        widget << "        <quad posn=\"77.5 -59.5 0.004\" sizen=\"3.5 3.5\" style=\"Icons64x64_1\" substyle=\"ArrowNext\" action=\"PageCenterList(" << (currentPage + 1) << ")\"/>";
        widget << "        <quad posn=\"80 -59.5 0.004\" sizen=\"3.5 3.5\" style=\"Icons64x64_1\" substyle=\"ArrowLast\" action=\"PageCenterList(" << pages << ")\"/>";
    }

    widget << "</frame>";

    frame.Page = widget.str();
    return Display(frame, player);
}

void UIManager::CloseList(Player player)
{
    currentLists.erase(player.Login);

    UIFrame frame = UIFrame();
    frame.ManiaLinkId = "CenterList";
    frame.Timeout = 1;
    frame.CloseOnClick = false;
    frame.Page = "";
    Display(frame, player);
}

void UIManager::OnEndMatch()
{
    for(std::map<std::string, Player>::iterator playerIt = players->begin(); playerIt != players->end(); ++playerIt)
    {
        CloseList(playerIt->second);
    }
}

bool UIManager::RegisterEvent(std::string answer, std::function<void(Player, std::string, std::vector<EntryVal>)> function)
{
    if(answers.find(answer) == answers.end())
    {
        answers.insert(std::pair<std::string, std::function<void(Player, std::string, std::vector<EntryVal>)>>(answer, function));
        return true;
    }
    else
    {
        std::cout << "[ WARNING ] Trying to register action '" << answer << "', which is already taken." << std::endl;
    }

    return false;
}

std::string UIManager::formatPage(UIFrame frame)
{
    std::stringstream page;
    page << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    page << "<manialink id=\"" << frame.ManiaLinkId << "\">";
    page << frame.Page;
    page << "</manialink>";

    if(frame.CustomUI.size() > 0)
    {
        page << "<custom_ui>";
        for(std::map<std::string, bool>::iterator customUI = frame.CustomUI.begin(); customUI != frame.CustomUI.end(); ++customUI)
        {
            std::string uiBool = (customUI->second) ? "true" : "false";
            page << "    <" << customUI->first << " visible=\"" << uiBool << "\" />";
        }
        page << "</custom_ui>";
    }

    return page.str();
}
