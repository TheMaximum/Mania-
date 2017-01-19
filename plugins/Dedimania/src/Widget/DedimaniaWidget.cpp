#include "DedimaniaWidget.h"

DedimaniaWidget::DedimaniaWidget()
{

}

DedimaniaWidget::DedimaniaWidget(UIManager* uiManager, std::vector<DediRecord>* dediRecords)
{
    ui = uiManager;
    records = dediRecords;

    frame = UIFrame();
    frame.ManiaLinkId = manialinkId;
    frame.Timeout = 0;
    frame.CloseOnClick = false;
}

bool DedimaniaWidget::DisplayToAll(std::map<std::string, Player>* players)
{
    bool response = true;

    for(std::map<std::string, Player>::iterator player = players->begin(); player != players->end(); ++player)
    {
        if(!DisplayToPlayer(player->second))
            response = false;
    }

    return response;
}

bool DedimaniaWidget::DisplayToPlayer(Player player)
{
    double widgetWidth = 15.5;
    double widgetHeight = ((1.8 * WidgetEntries) + 3.2);
    double columnHeight = (widgetHeight - 3.1);
    double backgroundWidth = (widgetWidth - 0.2);
    double backgroundHeight = (widgetHeight - 0.2);
    double borderWidth = (widgetWidth + 0.4);
    double borderHeight = (widgetHeight + 0.6);
    double columnNameWidth = (widgetWidth - 6.45);

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
    std::string backgroundRank = "06F5";
    std::string backgroundScore = "09F3";
    std::string backgroundName = "09F1";

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
    std::string iconStyle = "BgRaceScore2";
    std::string iconSubstyle = "LadderRank";

    std::string textColor = "FFFF";

    double topWidth = (widgetWidth - 0.8);
    double topHeight = ((WidgetTopCount * 1.8) + 0.2);
    std::string topStyle = "BgsPlayerCard";
    std::string topSubstyle = "BgCardSystem";

    double playerIconBoxX = (WidgetX < 0) ? widgetWidth : -2;
    double playerIconX = (WidgetX < 0) ? (widgetWidth + 0.2) : -1.8;
    std::string playerIcon = (WidgetX < 0) ? "ShowLeft2" : "ShowRight2";

    std::stringstream widget;
    widget << "    <frame posn=\"" << WidgetX << " " << WidgetY << " 0\" id=\"Widget" << manialinkId << "\">";
    widget << "        <label posn=\"0.1 -0.1 0\" sizen=\"" << backgroundWidth << " " << backgroundHeight << "\" action=\"" << actionId << "\" text=\" \" focusareacolor1=\"" << backgroundColor << "\" focusareacolor2=\"" << backgroundFocus << "\"/>";
    widget << "        <quad posn=\"-0.2 0.3 0.001\" sizen=\"" << borderWidth << " " << borderHeight << "\" style=\"" << borderStyle << "\" substyle=\"" << borderSubstyle << "\"/>";
    widget << "        <quad posn=\"0 0 0.002\" sizen=\"" << backgroundWidth << " " << backgroundHeight << "\" style=\"" << backgroundStyle << "\" substyle=\"" << backgroundSubstyle << "\"/>";

    widget << "        <quad posn=\"0.4 -2.6 0.003\" sizen=\"2 " << columnHeight << "\" bgcolor=\"" << backgroundRank << "\"/>";
    widget << "        <quad posn=\"2.4 -2.6 0.003\" sizen=\"3.65 " << columnHeight << "\" bgcolor=\"" << backgroundScore << "\"/>";
    widget << "        <quad posn=\"6.05 -2.6 0.003\" sizen=\"" << columnNameWidth << " " << columnHeight << "\" bgcolor=\"" << backgroundName << "\"/>";

    widget << "        <quad posn=\"" << imageOpenX << " " << imageOpenY << " 0.05\" sizen=\"3.5 3.5\" image=\"" << imageOpen << "\"/>";

    widget << "        <quad posn=\"0.4 -0.36 0.003\" sizen=\"" << titleBackgroundWidth << " 2\" style=\"" << titleStyle << "\" substyle=\"" << titleSubstyle << "\"/>";
    widget << "        <quad posn=\"" << iconX << " " << iconY << " 0.004\" sizen=\"2.5 2.5\" style=\"" << iconStyle << "\" substyle=\"" << iconSubstyle << "\"/>";
    widget << "        <label posn=\"" << titleX << " " << titleY << " 0.004\" sizen=\"10.2 0\" halign=\"" << titleHalign << "\" textsize=\"1\" text=\"" << title << "\"/>";
    widget << "        <format textsize=\"1\" textcolor=\"" << textColor << "\"/>";

    widget << "        <quad posn=\"0.4 -2.6 0.004\" sizen=\"" << topWidth << " " << topHeight << "\" style=\"" << topStyle << "\" substyle=\"" << topSubstyle << "\"/>";

    double recordY = -3;

    std::string scoreColor = "F00F";
    std::string topColor = "FF0F";

    for(int recordId = 0; recordId < records->size(); recordId++)
    {
        DediRecord record = records->at(recordId);
        if(record.Login == player.Login)
        {
            scoreColor = "0F3F";
            topColor = "0F3F";
            widget << "        <quad posn=\"" << playerIconBoxX << " " << (recordY + 0.35) << " 0.004\" sizen=\"2 1.95\" style=\"BgsPlayerCard\" substyle=\"BgCardSystem\"/>";
            widget << "        <quad posn=\"" << playerIconX << " " << (recordY + 0.15) << " 0.005\" sizen=\"1.6 1.6\" style=\"Icons64x64_1\" substyle=\"" << playerIcon << "\"/>";
        }

        widget << "        <label posn=\"2.3 " << recordY << " 0.005\" sizen=\"1.7 1.7\" halign=\"right\" scale=\"0.9\" text=\"" << (recordId + 1) << ".\"/>";
        widget << "        <label posn=\"6 " << recordY << " 0.005\" sizen=\"3.9 1.7\" halign=\"right\" scale=\"0.9\" textcolor=\"" << topColor << "\" text=\"" << record.FormattedTime << "\"/>";
        widget << "        <label posn=\"6.2 " << recordY << " 0.005\" sizen=\"9.50 1.7\" scale=\"0.9\" text=\"" << Text::EscapeXML(record.NickName) << "\"/>";

        if(record.Login == player.Login)
        {
            scoreColor = "999F";
            topColor = "FF0F";
        }

        recordY -= 1.8;

        if((recordId + 2) > WidgetTopCount) break;
    }

    int recordCount = records->size();
    int startPoint = (recordCount - (WidgetEntries - WidgetTopCount));
    if(startPoint < WidgetTopCount)
        startPoint = WidgetTopCount;
    int playerRecord = 0;
    for(int recordId = 0; recordId < records->size(); recordId++)
    {
        DediRecord record = records->at(recordId);
        if(record.Login == player.Login)
        {
            playerRecord = record.Rank;
        }
    }

    if(playerRecord > 0)
    {
        int recordsToFill = ((WidgetEntries - WidgetTopCount) - 1);
        int fillAbove = std::ceil(recordsToFill / 2.0);
        int fillBelow = std::floor(recordsToFill / 2.0);
        int roomAbove = (playerRecord - WidgetTopCount);
        int roomBelow = (recordCount - playerRecord);

        if(fillBelow > roomBelow)
        {
            fillAbove += (fillBelow - roomBelow);
        }

        startPoint = ((playerRecord - fillAbove) - 1);
        if(startPoint < WidgetTopCount)
            startPoint = WidgetTopCount;
    }

    for(int recordId = startPoint; recordId < (startPoint + (WidgetEntries - WidgetTopCount)); recordId++)
    {
        if(recordId > records->size())
            break;
            
        DediRecord record = records->at(recordId);
        if(record.Login == player.Login)
        {
            scoreColor = "0F3F";
            widget << "        <quad posn=\"" << playerIconBoxX << " " << (recordY + 0.35) << " 0.004\" sizen=\"2 1.95\" style=\"BgsPlayerCard\" substyle=\"BgCardSystem\"/>";
            widget << "        <quad posn=\"" << playerIconX << " " << (recordY + 0.15) << " 0.005\" sizen=\"1.6 1.6\" style=\"Icons64x64_1\" substyle=\"" << playerIcon << "\"/>";
            widget << "        <quad posn=\"0.4 " << (recordY + 0.35) << " 0.004\" sizen=\"" << topWidth << " 1.95\" style=\"" << topStyle << "\" substyle=\"" << topSubstyle << "\"/>";
        }

        widget << "        <label posn=\"2.3 " << recordY << " 0.005\" sizen=\"1.7 1.7\" halign=\"right\" scale=\"0.9\" text=\"" << (recordId + 1) << ".\"/>";
        widget << "        <label posn=\"6 " << recordY << " 0.005\" sizen=\"3.9 1.7\" halign=\"right\" scale=\"0.9\" textcolor=\"" << scoreColor << "\" text=\"" << record.FormattedTime << "\"/>";
        widget << "        <label posn=\"6.2 " << recordY << " 0.005\" sizen=\"9.50 1.7\" scale=\"0.9\" text=\"" << Text::EscapeXML(record.NickName) << "\"/>";

        if(record.Login == player.Login)
        {
            scoreColor = "BBBF";
        }

        recordY -= 1.8;

        if((recordId + 2) > recordCount) break;
    }

    widget << "    </frame>";

    frame.Page = widget.str();
    return ui->Display(frame, player);
}
