#include "DedimaniaPlugin.h"

DedimaniaPlugin::DedimaniaPlugin()
{
    Version = "0.1.0";
    Author = "TheM";

    BeginMap.push_back([this](Map map) { OnBeginMap(); });
    PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
    RegisterCommand("dedirecs", [this](Player player, std::vector<std::string> parameters) { OpenDediRecords(player); });
    RegisterCommand("dedirecords", [this](Player player, std::vector<std::string> parameters) { OpenDediRecords(player); });

    curl_global_init(CURL_GLOBAL_ALL);
}

void DedimaniaPlugin::Init()
{
    loadSettings();
    authenticate();

    widget = DedimaniaWidget(controller->UI, &records);
    widget.WidgetEntries = widgetEntries;
    widget.WidgetTopCount = widgetTopCount;
    widget.WidgetX = widgetX;
    widget.WidgetY = widgetY;
    controller->UI->RegisterEvent("OpenDediRecords", ([this](Player player, std::string answer, std::vector<EntryVal> entries) { OpenDediRecords(player); }));

    OnBeginMap();
}

void DedimaniaPlugin::OnBeginMap()
{
    Map currentMap = *controller->Maps->Current;

    if(sessionId != "")
    {
        std::cout << "[         ] Retrieving Dedimania records for current map ... " << std::endl << std::flush;

        std::stringstream getRecords;
        getRecords << "<member><name>methodName</name><value><string>dedimania.GetChallengeRecords</string></value></member>";
        getRecords << "<member><name>params</name><value><array><data>";
        getRecords << "<member><name>SessionId</name><value><string>" << sessionId << "</string></value></member>";
        getRecords << "<member><name>MapInfo</name><value><struct>";
        getRecords << "<member><name>UId</name><value><string>" << currentMap.UId << "</string></value></member>";
        getRecords << "<member><name>Name</name><value><string>" << currentMap.Name << "</string></value></member>";
        getRecords << "<member><name>Environment</name><value><string>" << currentMap.Environment << "</string></value></member>";
        getRecords << "<member><name>Author</name><value><string>" << currentMap.Author << "</string></value></member>";
        getRecords << "<member><name>NbCheckpoints</name><value><int>" << currentMap.NbCheckpoints << "</int></value></member>";
        getRecords << "<member><name>NbLaps</name><value><int>" << currentMap.NbLaps << "</int></value></member>";
        getRecords << "</struct></value></member>";
        // !!! Make work with actual values!!!
        getRecords << "<member><name>GameMode</name><value><string>TA</string></value></member>";
        getRecords << "<member><name>SrvInfo</name><value><struct>";
        getRecords << "<member><name>SrvName</name><value><string>Testserver</string></value></member>";
        getRecords << "<member><name>Comment</name><value><string>Teest</string></value></member>";
        getRecords << "<member><name>Private</name><value><boolean>1</boolean></value></member>";
        getRecords << "<member><name>NumPlayers</name><value><int>0</int></value></member>";
        getRecords << "<member><name>MaxPlayers</name><value><int>20</int></value></member>";
        getRecords << "<member><name>NumSpecs</name><value><int>0</int></value></member>";
        getRecords << "<member><name>MaxSpecs</name><value><int>20</int></value></member>";
        getRecords << "</struct></value></member>";
        getRecords << "<member><name>Players</name><value><array><data></data></array></value></member>";
        getRecords << "</data></array></value></member>";
        currentCalls.Put(getRecords.str());

        GbxResponse queryResponse = multicall();
        if(!hasError)
        {
            std::vector<GbxResponseParameter> responseParams = queryResponse.GetParameters().at(0).GetArray().at(0).GetArray();
            std::map<std::string, GbxResponseParameter> responseStruct = responseParams.at(0).GetStruct();
            maxRank = atoi(responseStruct.find("ServerMaxRank")->second.GetString().c_str());
            std::vector<GbxResponseParameter> recordsStruct = responseStruct.find("Records")->second.GetArray();

            records = std::vector<DediRecord>();
            for(int recordId = 0; recordId < recordsStruct.size(); recordId++)
            {
                std::map<std::string, GbxResponseParameter> recordStruct = recordsStruct.at(recordId).GetStruct();
                DediRecord record = DediRecord(recordStruct);

                records.push_back(record);

                if(record.Rank == maxRank)
                    break;
            }

            std::cout << "\x1b[1A[   \033[0;32mOK.\033[0;0m   ] Successfully retrieved Dedimania records for current map!" << std::endl << std::flush;
        }
        else
        {
            std::cout << "[ \033[0;31mFAILED!\033[0;0m ] Unable to retrieve records from Dedimania!" << std::endl << std::flush;
        }
    }

    if(!widget.DisplayToAll(controller->Players))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void DedimaniaPlugin::OnPlayerConnect(Player player)
{
    if(!widget.DisplayToPlayer(player))
    {
        Logging::PrintError(controller->Server->GetCurrentError());
    }
}

void DedimaniaPlugin::OpenDediRecords(Player player)
{
    UIList list = UIList();
    list.Id = "DediRecords";
    list.Title = "$l[http://dedimania.net/tm2stats/?do=stat&Envir=" + controller->Maps->Current->Environment + "&RecOrder3=RANK-ASC&UId=" + controller->Maps->Current->UId + "&Show=RECORDS]Dedimania Records$l for: $z$s$fff" + controller->Maps->Current->Name;
    list.IconStyle = "BgRaceScore2";
    list.IconSubstyle = "LadderRank";
    list.Columns.push_back(std::pair<std::string, int>("#", 5));
    list.Columns.push_back(std::pair<std::string, int>("Player", 40));
    list.Columns.push_back(std::pair<std::string, int>("Time", 20));

    for(int recordId = 0; recordId < records.size(); recordId++)
    {
        DediRecord record = records.at(recordId);

        std::map<std::string, std::string> row = std::map<std::string, std::string>();
        row.insert(std::pair<std::string, std::string>("#", std::to_string(record.Rank)));
        row.insert(std::pair<std::string, std::string>("Player", record.NickName));
        row.insert(std::pair<std::string, std::string>("Time", record.FormattedTime));
        list.Rows.push_back(row);
    }

    controller->UI->DisplayList(list, player);
}

void DedimaniaPlugin::loadSettings()
{
    std::map<std::string, std::string>::iterator limitIt = Settings.find("code");
    if(limitIt != Settings.end())
        dedimaniaCode = limitIt->second;

    std::map<std::string, std::string>::iterator widgetEntriesIt = Settings.find("widgetEntries");
    if(widgetEntriesIt != Settings.end())
        widgetEntries = atoi(widgetEntriesIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetTopCountIt = Settings.find("widgetTopCount");
    if(widgetTopCountIt != Settings.end())
        widgetTopCount = atoi(widgetTopCountIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetXIt = Settings.find("widgetX");
    if(widgetXIt != Settings.end())
        widgetX = atof(widgetXIt->second.c_str());

    std::map<std::string, std::string>::iterator widgetYIt = Settings.find("widgetY");
    if(widgetYIt != Settings.end())
        widgetY = atof(widgetYIt->second.c_str());
}

void DedimaniaPlugin::authenticate()
{
    std::cout << "[         ] Connecting with Dedimania on dedimania.net:8081 ... " << std::endl << std::flush;

    std::stringstream openSession;
    openSession << "<member><name>methodName</name><value><string>dedimania.OpenSession</string></value></member>";
    openSession << "<member><name>params</name><value><array><data><value><struct>";
    openSession << "<member><name>Game</name><value><string>TM2</string></value></member>";
    openSession << "<member><name>Login</name><value><string>" << controller->Info->System.ServerLogin << "</string></value></member>";
    openSession << "<member><name>Code</name><value><string>" << dedimaniaCode << "</string></value></member>";
    openSession << "<member><name>Path</name><value><string>World|" << controller->Info->Account.Path << "</string></value></member>";
    openSession << "<member><name>Packmask</name><value><string>" << controller->Maps->Current->Environment << "</string></value></member>";
    openSession << "<member><name>ServerVersion</name><value><string>" << controller->Info->Version.Version << "</string></value></member>";
    openSession << "<member><name>ServerBuild</name><value><string>" << controller->Info->Version.Build << "</string></value></member>";
    openSession << "<member><name>Tool</name><value><string>Mania++</string></value></member>";
    openSession << "<member><name>Version</name><value><string>" << controller->Info->ControllerVersion << "</string></value></member>";
    openSession << "</struct></value></data></array></value></member>";
    currentCalls.Put(openSession.str());

    GbxResponse queryResponse = multicall();
    if(!hasError)
    {
        std::vector<GbxResponseParameter> responseParams = queryResponse.GetParameters().at(0).GetArray();
        sessionId = responseParams.at(0).GetArray().at(0).GetStruct().at("SessionId").GetString();

        std::cout << "\x1b[1A[   \033[0;32mOK.\033[0;0m   ] Connected with Dedimania (session: " << sessionId << ")!" << std::endl << std::flush;
    }
    else
    {
        std::cout << "[ \033[0;31mFAILED!\033[0;0m ] Failed to connect with Dedimania!" << std::endl << std::flush;
    }
}

size_t DedimaniaPlugin::receiveData(void* contents, size_t size, size_t nmemb, std::string* s)
{
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try
    {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }

    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

GbxResponse DedimaniaPlugin::multicall()
{
    GbxParameters params = GbxParameters();
    GbxParameters structArray = GbxParameters();
    std::stringstream warnings;
    warnings << "<member><name>methodName</name><value><string>dedimania.WarningsAndTTR</string></value></member>";
    warnings << "<member><name>params</name><value><array><data></data></array></value></member>";
    currentCalls.Put(warnings.str());
    structArray.Put(&currentCalls);
    params.Put(&structArray);

    GbxResponse response = query(GbxMessage("system.multicall", params));
    currentCalls = GbxStructParameters();
    return response;
}

GbxResponse DedimaniaPlugin::query(GbxMessage message)
{
    hasError = false;
    std::string output;
    CURLcode result;
    std::string tags;

    std::string text = message.GetXml();
    //std::cout << "Text: " << text << std::endl;
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(text.c_str()), text.length());
    //std::cout << "Encoded: " << encoded << std::endl;

    std::stringstream postfields;
    postfields << "xmlrpc=" << encoded;

    curl = curl_easy_init();

    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, "Cache-Control: no-cache");
    headers = curl_slist_append(headers, "Keep-Alive: timeout=600, max=2000");
    headers = curl_slist_append(headers, "Connection: Keep-Alive");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");

    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_URL, "dedimania.net:8081/Dedimania");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mania++/0.3.0");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate,gzip");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DedimaniaPlugin::receiveData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, strlen(postfields.str().c_str()));
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postfields.str().c_str());

    result = curl_easy_perform(curl);
    if(result != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);

    GbxResponse response;
    response.SetRaw(output);

    //std::cout << "Response: " << output << std::endl;

    std::vector<GbxResponseParameter> responseParams = response.GetParameters().at(0).GetArray();
    std::map<std::string, GbxResponseParameter> warningsAndTTR = responseParams.at((responseParams.size() - 1)).GetArray().at(0).GetStruct();
    std::vector<GbxResponseParameter> warnings = warningsAndTTR.at("methods").GetArray();
    for(int paramId = 0; paramId < warnings.size(); paramId++)
    {
        std::map<std::string, GbxResponseParameter> warning = warnings.at(paramId).GetStruct();
        if(warning.at("errors").GetString().length() > 1)
        {
            hasError = true;
            std::stringstream errorMessage;
            errorMessage << "(" << warning.at("methodName").GetString() << ") " << warning.at("errors").GetString();
            Logging::PrintError(-200, errorMessage.str());
        }
    }

    return response;
}
