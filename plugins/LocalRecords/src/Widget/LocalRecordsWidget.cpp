#include "LocalRecordsWidget.h"

LocalRecordsWidget::LocalRecordsWidget()
{
    frame = UIFrame();
    frame.Timeout = 0;
    frame.CloseOnClick = false;
}

LocalRecordsWidget::LocalRecordsWidget(UIManager* uiManager, std::vector<LocalRecord>* localRecords)
{
    ui = uiManager;
    records = localRecords;
}

bool LocalRecordsWidget::DisplayToAll(std::map<std::string, Player>* players)
{
    bool response = true;

    for(std::map<std::string, Player>::iterator player = players->begin(); player != players->end(); ++player)
    {
        if(!DisplayToPlayer(player->second))
            response = false;
    }

    return response;
}

bool LocalRecordsWidget::DisplayToPlayer(Player player)
{
    std::stringstream widget;
    widget << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    widget << "<manialink id=\"" << manialinkId << "\">";
    widget << "    <frame posn=\"" << widgetX << " " << widgetY << " 0\" id=\"Widget" << manialinkId << "\">";
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

    for(int recordId = 0; recordId < records->size(); recordId++)
    {
        LocalRecord record = records->at(recordId);
        std::string scoreColor = "FF0F";
        if(record.Login == player.Login)
        {
            scoreColor = "0F3F";
            widget << "        <quad posn=\"" << playerIconBoxX << " " << (recordY + 0.35) << " 0.004\" sizen=\"2 1.95\" style=\"BgsPlayerCard\" substyle=\"BgCardSystem\"/>";
            widget << "        <quad posn=\"" << playerIconX << " " << (recordY + 0.15) << " 0.005\" sizen=\"1.6 1.6\" style=\"Icons64x64_1\" substyle=\"{{playericon}}\"/>";
        }

        widget << "        <label posn=\"2.3 " << recordY << " 0.005\" sizen=\"1.7 1.7\" halign=\"right\" scale=\"0.9\" text=\"" << (recordId + 1) << ".\"/>";
        widget << "        <label posn=\"6 " << recordY << " 0.005\" sizen=\"3.9 1.7\" halign=\"right\" scale=\"0.9\" textcolor=\"FFFF\" text=\"" << record.FormattedTime << "\"/>";
        widget << "        <label posn=\"6.2 " << recordY << " 0.005\" sizen=\"9.50 1.7\" scale=\"0.9\" text=\"" << record.NickName << "\"/>";

        recordY -= 1.8;
        if((recordId + 2) > widgetEntries) break;
    }

    widget << "    </frame>";
    widget << "</manialink>";

    frame.Page = widget.str();

    return ui->Display(frame, player);
}
