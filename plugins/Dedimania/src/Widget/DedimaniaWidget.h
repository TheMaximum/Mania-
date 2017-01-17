#ifndef DEDIMANIAWIDGET_H_
#define DEDIMANIAWIDGET_H_

#include <string>
#include <vector>
#include <map>

#include "Methods/Methods.h"
#include "Objects/Player.h"
#include "UI/UIManager.h"
#include "Utils/Logging.h"
#include "Utils/Text.h"

#include "../Objects/DediRecord.h"

class DedimaniaWidget
{
public:
    DedimaniaWidget();
    DedimaniaWidget(UIManager* uiManager, std::vector<DediRecord>* localRecords);
    bool DisplayToAll(std::map<std::string, Player>* players);
    bool DisplayToPlayer(Player player);

    int WidgetEntries = 28;
    int WidgetTopCount = 5;
    double WidgetX = -64.7;
    double WidgetY = 28.2;

private:
    UIManager* ui;
    std::vector<DediRecord>* records;

    UIFrame frame;

    std::string manialinkId = "Dedimania";
    std::string title = "Dedimania Records";
    std::string actionId = "OpenDediRecords";
};

#endif // DEDIMANIAWIDGET_H_
