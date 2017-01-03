#include "MapWidget.h"

MapWidget::MapWidget()
{

}

MapWidget::MapWidget(UIManager* uiManager)
{
    ui = uiManager;

    frame = UIFrame();
    frame.ManiaLinkId = manialinkId;
    frame.Timeout = 0;
    frame.CloseOnClick = false;
}

bool MapWidget::DisplayToAll(std::map<std::string, Player>* players, Map* currentMap)
{
    bool response = true;

    for(std::map<std::string, Player>::iterator player = players->begin(); player != players->end(); ++player)
    {
        if(!DisplayToPlayer(player->second, currentMap))
            response = false;
    }

    return response;
}

bool MapWidget::DisplayToPlayer(Player player, Map* currentMap)
{
    std::stringstream widget;
    widget << "    <frame posn=\"" << widgetX << " " << widgetY << " 0\" id=\"Widget" << manialinkId << "\">";
    widget << "        <label posn=\"0.1 -0.1 0\" sizen=\"" << backgroundWidth << " " << backgroundHeight << "\" text=\" \" focusareacolor1=\"" << backgroundColor << "\" focusareacolor2=\"" << backgroundFocus << "\"/>";
    widget << "        <quad posn=\"-0.2 0.3 0.001\" sizen=\"" << borderWidth << " " << borderHeight << "\" style=\"" << borderStyle << "\" substyle=\"" << borderSubstyle << "\"/>";
    widget << "        <quad posn=\"0 0 0.002\" sizen=\"" << backgroundWidth << " " << backgroundHeight << "\" style=\"" << backgroundStyle << "\" substyle=\"" << backgroundSubstyle << "\"/>";

    widget << "        <quad posn=\"0.4 -0.36 0.003\" sizen=\"" << titleBackgroundWidth << " 2\" style=\"" << titleStyle << "\" substyle=\"" << titleSubstyle << "\"/>";
    widget << "        <quad posn=\"" << iconX << " " << iconY << " 0.004\" sizen=\"2.5 2.5\" style=\"" << iconStyle << "\" substyle=\"" << iconSubstyle << "\"/>";
    widget << "        <label posn=\"" << titleX << " " << titleY << " 0.004\" sizen=\"10.2 0\" halign=\"" << titleHalign << "\" textsize=\"1\" text=\"" << Title << "\"/>";
    widget << "        <format textsize=\"1\" textcolor=\"" << textColor << "\"/>";

    widget << "        <label posn=\"1 -2.7 0.04\" sizen=\"13.55 2\" scale=\"1\" text=\"" << currentMap->Name << "\"/>";
    widget << "        <quad posn=\"0.7 -4.35 0.04\" sizen=\"1.7 1.7\" style=\"Icons128x128_1\" substyle=\"ChallengeAuthor\"/>";
    widget << "        <label posn=\"2.7 -4.6 0.04\" sizen=\"13 2\" scale=\"0.85\" text=\"" << currentMap->Author << "\"/>";

    widget << "        <quad posn=\"0.7 -6.25 0.04\" sizen=\"1.7 1.7\" style=\"BgRaceScore2\" substyle=\"ScoreReplay\"/>";
    widget << "        <label posn=\"2.7 -6.55 0.04\" sizen=\"4.5 2\" scale=\"0.85\" text=\"" << Time::FormatTime(currentMap->AuthorTime) << "\"/>";

    widget << "        <quad posn=\"7.2 -6.25 0.04\" sizen=\"1.7 1.7\" style=\"Icons128x128_1\" substyle=\"United\"/>";
    widget << "        <label posn=\"9.2 -6.55 0.04\" sizen=\"4.5 2\" scale=\"0.85\" text=\"" << currentMap->Environment << "\"/>";

    widget << "    </frame>";

    frame.Page = widget.str();
    return ui->Display(frame, player);
}
