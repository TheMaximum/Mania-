#include "UIManager.h"

UIManager::UIManager(Methods* serverPtr, EventManager* eventsPtr)
{
    server = serverPtr;
    events = eventsPtr;
    answers = std::map<std::string, std::function<void(Player, std::string, std::vector<EntryVal>)>>();

    std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>> functions = std::vector<std::function<void(Player, std::string, std::vector<EntryVal>)>>();
    functions.push_back([this](Player player, std::string answer, std::vector<EntryVal> entries) { OnPlayerManialinkPageAnswer(player, answer, entries); });
    events->RegisterPlayerManialinkPageAnswer(functions);
}

void UIManager::OnPlayerManialinkPageAnswer(Player player, std::string answer, std::vector<EntryVal> entries)
{
    if(answers.find(answer) != answers.end())
    {
        std::function<void(Player, std::string, std::vector<EntryVal>)> method = answers.find(answer)->second;
        method(player, answer, entries);
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

bool UIManager::AddEvent(std::string answer, std::function<void(Player, std::string, std::vector<EntryVal>)> function)
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
