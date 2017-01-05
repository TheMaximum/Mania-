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
    frame.CustomUI.insert(std::pair<std::string, bool>("challenge_info", false));
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
    double widgetWidth = 15.5;
    double widgetHeight = 8.9;
    double columnHeight = (widgetHeight - 3.1);
    double backgroundWidth = (widgetWidth - 0.2);
    double backgroundHeight = (widgetHeight - 0.2);
    double borderWidth = (widgetWidth + 0.4);
    double borderHeight = (widgetHeight + 0.6);

    double left_IconX = 0.6;
    double left_IconY = 0;
    double left_TitleX = 3.2;
    double left_TitleY = -0.65;
    std::string left_TitleHalign = "left";
    double left_ImageOpenX = -0.3;
    std::string left_ImageOpen = "http://static.undef.name/ingame/records-eyepiece/edge-open-ld-dark.png";

    double right_IconX = 12.5;
    double right_IconY = 0;
    double right_TitleX = 12.4;
    double right_TitleY = -0.65;
    std::string right_TitleHalign = "right";
    double right_ImageOpenX = 12.2;
    std::string right_ImageOpen = "http://static.undef.name/ingame/records-eyepiece/edge-open-rd-dark.png";

    std::string backgroundColor = "3342";
    std::string backgroundFocus = "09F6";

    std::string backgroundStyle = "Bgs1";
    std::string backgroundSubstyle = "BgTitleGlow";
    std::string borderStyle = "Bgs1";
    std::string borderSubstyle = "BgTitleShadow";

    double imageOpenX = (WidgetX < 0) ? (right_ImageOpenX + (widgetWidth - 15.5)) : left_ImageOpenX;
    double imageOpenY = -(widgetHeight - 3.18);
    std::string imageOpen = (WidgetX < 0) ? right_ImageOpen : left_ImageOpen;

    double titleBackgroundWidth = (widgetWidth - 0.8);
    std::string titleStyle = "BgsPlayerCard";
    std::string titleSubstyle = "BgRacePlayerName";
    double titleX = (WidgetX < 0) ? (right_TitleX + (widgetWidth - 15.5)) : left_TitleX;
    double titleY = (WidgetX < 0) ? right_TitleY : left_TitleY;
    std::string titleHalign = (WidgetX < 0) ? right_TitleHalign : left_TitleHalign;

    double iconX = (WidgetX < 0) ? (right_IconX + (widgetWidth - 15.5)) : left_IconX;
    double iconY = (WidgetX < 0) ? right_IconY : left_IconY;
    std::string iconStyle = "Icons128x128_1";
    std::string iconSubstyle = "Challenge";

    std::string textColor = "FFFF";

    std::stringstream widget;
    widget << "    <frame posn=\"" << WidgetX << " " << WidgetY << " 0\" id=\"Widget" << manialinkId << "\">";
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
