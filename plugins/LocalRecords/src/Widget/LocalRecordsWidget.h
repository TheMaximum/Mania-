#ifndef LOCALRECORDSWIDGET_H_
#define LOCALRECORDSWIDGET_H_

#include <string>
#include <vector>
#include <map>

#include "Methods/Methods.h"
#include "Objects/Player.h"
#include "UI/UIManager.h"
#include "Utils/Logging.h"
#include "Utils/Text.h"

#include "../Objects/LocalRecord.h"
#include "../LocalRecordsList.h"

class LocalRecordsWidget
{
public:
    LocalRecordsWidget();
    LocalRecordsWidget(UIManager* uiManager, LocalRecordsList* localRecords);
    bool DisplayToAll(std::map<std::string, Player>* players);
    bool DisplayToPlayer(Player player);

    int WidgetEntries = 28;
    int WidgetTopCount = 5;
    double WidgetX = 49.2;
    double WidgetY = 28.2;

private:
    UIManager* ui;
    LocalRecordsList* records;

    UIFrame frame;

    std::string manialinkId = "LocalRecords";
    std::string title = "Local Records";
    std::string actionId = "OpenLocalRecords";
};

#endif // LOCALRECORDSWIDGET_H_
