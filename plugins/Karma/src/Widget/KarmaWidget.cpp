#include "KarmaWidget.h"

KarmaWidget::KarmaWidget()
{

}

KarmaWidget::KarmaWidget(UIManager* uiManager)
{
    ui = uiManager;

    frame = UIFrame();
    frame.ManiaLinkId = manialinkId;
    frame.Timeout = 0;
    frame.CloseOnClick = false;
}

bool KarmaWidget::DisplayToPlayer(Player player, MapKarma* karma, int personalScore)
{
    double widgetWidth = 15.5;
    double widgetHeight = 11;
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
    std::string iconSubstyle = "CustomStars";

    std::string textColor = "FFFF";

    std::stringstream widget;
    widget << "    <frame posn=\"" << WidgetX << " " << WidgetY << " 0\" id=\"Widget" << manialinkId << "\">";

    widget << "        <label posn=\"0.1 -0.1 0\" sizen=\"" << backgroundWidth << " " << backgroundHeight << "\" text=\" \" action=\"" << ActionId << "\" focusareacolor1=\"" << backgroundColor << "\" focusareacolor2=\"" << backgroundFocus << "\"/>";
    widget << "        <quad posn=\"-0.2 0.3 0.001\" sizen=\"" << borderWidth << " " << borderHeight << "\" style=\"" << borderStyle << "\" substyle=\"" << borderSubstyle << "\"/>";
    widget << "        <quad posn=\"0 0 0.002\" sizen=\"" << backgroundWidth << " " << backgroundHeight << "\" style=\"" << backgroundStyle << "\" substyle=\"" << backgroundSubstyle << "\"/>";

    widget << "        <quad posn=\"" << imageOpenX << " " << imageOpenY << " 0.05\" sizen=\"3.5 3.5\" image=\"" << imageOpen << "\"/>";

    widget << "        <quad posn=\"0.4 -0.36 0.003\" sizen=\"" << titleBackgroundWidth << " 2\" style=\"" << titleStyle << "\" substyle=\"" << titleSubstyle << "\"/>";
    widget << "        <quad posn=\"" << iconX << " " << iconY << " 0.004\" sizen=\"2.5 2.5\" style=\"" << iconStyle << "\" substyle=\"" << iconSubstyle << "\"/>";
    widget << "        <label posn=\"" << titleX << " " << titleY << " 0.004\" sizen=\"10.2 0\" halign=\"" << titleHalign << "\" textsize=\"1\" text=\"" << title << "\"/>";
    widget << "        <format textsize=\"1\" textcolor=\"" << textColor << "\"/>";

    if(personalScore == 0)
        widget << "        <quad posn=\"1.75 -5.25 1.6\" sizen=\"3.5 3.5\" style=\"Icons64x64_1\" substyle=\"LvlRed\"/>";

    widget << "        <quad posn=\"2 -5.5 1.75\" sizen=\"3 3\" style=\"Icons64x64_1\" substyle=\"Sub\" action=\"" << NegativeAction << "\" />";
    widget << "        <label posn=\"3.5 -8.5 1.75\" halign=\"center\" sizen=\"3 1\" text=\"$f00" << karma->MinVotes << "\" />";

    if(personalScore == 1)
        widget << "        <quad posn=\"9.75 -5.25 1.6\" sizen=\"3.5 3.5\" style=\"Icons64x64_1\" substyle=\"LvlGreen\"/>";

    widget << "        <quad posn=\"10 -5.5 1.75\" sizen=\"3 3\" style=\"Icons64x64_1\" substyle=\"Add\" action=\"" << PositiveAction << "\" />";
    widget << "        <label posn=\"11.5 -8.5 1.75\" halign=\"center\" sizen=\"3 1\"  text=\"$0f0" << karma->PlusVotes << "\"/>";

    widget << "        <label posn=\"7.5 -8.5 1.75\" halign=\"center\" sizen=\"5 1\" text=\"$fff" << karma->Percentage << "%\"/>";

    int mapKarma = (karma->PlusVotes - karma->MinVotes);
    std::string colour = "$fff";
    std::string prefix = "";
    if(mapKarma < 0)
    {
        colour = "$f00";
    }
    else if(mapKarma > 0)
    {
        colour = "$0f0";
        prefix = "+";
    }

    widget << "        <label posn=\"7.5 -6.35 1.75\" halign=\"center\" sizen=\"5 1\" text=\"" << colour << "$o" << prefix << mapKarma << "\"/>";

    double barX = 1.5;
    if(karma->Percentage == 0)
    {
        for(int i = 0; i < 10; i++)
        {
            widget << "        <quad posn=\"" << barX << " -3 1.5\" sizen=\"1.2 2.4\" image=\"http://kreipe.patrick.coolserverhosting.de/local/grey.jpg\"/>";
            barX += 1.2;
        }
    }
    else
    {
        int plus = std::round(((double)karma->PlusVotes / ((double)karma->PlusVotes + (double)karma->MinVotes)) * 10);
        for(int i = 0; i < plus; i++)
        {
            widget << "        <quad posn=\"" << barX << " -3 1.5\" sizen=\"1.2 2.4\" image=\"http://kreipe.patrick.coolserverhosting.de/local/green.jpg\"/>";
            barX += 1.2;
        }

        for(int i = plus; i < 10; i++)
        {
            widget << "        <quad posn=\"" << barX << " -3 1.5\" sizen=\"1.2 2.4\" image=\"http://kreipe.patrick.coolserverhosting.de/local/red.jpg\"/>";
            barX += 1.2;
        }
    }

    widget << "    </frame>";

    frame.Page = widget.str();
    return ui->Display(frame, player);
}
