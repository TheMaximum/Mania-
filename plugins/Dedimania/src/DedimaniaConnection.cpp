#include "DedimaniaPlugin.h"

void DedimaniaPlugin::authenticate()
{
    std::cout << "[          ] Connecting with Dedimania on dedimania.net:8081 ... " << std::endl << std::flush;

    GbxStructParameters session = GbxStructParameters();
    std::string methodName = "dedimania.OpenSession";
    session.Put("methodName", Parameter(&methodName));
    GbxStructParameters params = GbxStructParameters();
    std::string game = "TM2";
    std::string tool = "Mania++";
    params.Put("Game", Parameter(&game));
    params.Put("Login", Parameter(&controller->Info->System.ServerLogin));
    params.Put("Code", Parameter(&dedimaniaCode));
    params.Put("Path", Parameter(&controller->Info->Account.Path));
    params.Put("Packmask", Parameter(&controller->Maps->Current->Environment));
    params.Put("ServerVersion", Parameter(&controller->Info->Version.Version));
    params.Put("ServerBuild", Parameter(&controller->Info->Version.Build));
    params.Put("Tool", Parameter(&tool));
    params.Put("Version", Parameter(&controller->Info->ControllerVersion));
    GbxParameters parameters = GbxParameters();
    parameters.Put(&params);
    session.Put("params", Parameter(&parameters));
    currentCalls.push_back(session);

    GbxResponse queryResponse = multicall();
    if(!hasError)
    {
        std::vector<GbxResponseParameter> responseParams = queryResponse.GetParameters().at(0).GetArray();
        sessionId = responseParams.at(0).GetArray().at(0).GetStruct().at("SessionId").GetString();

        std::cout << "\x1b[1A[    \033[0;32mOK\033[0;0m    ] Connected with Dedimania (session: " << sessionId << ")!" << std::endl << std::flush;
    }
    else
    {
        std::cout << "[  \033[0;31mFAILED\033[0;0m  ] Failed to connect with Dedimania!" << std::endl << std::flush;
    }
}

GbxResponse DedimaniaPlugin::multicall()
{
    GbxParameters params = GbxParameters();
    GbxParameters structArray = GbxParameters();

    GbxStructParameters ttr = GbxStructParameters();
    std::string methodName = "dedimania.WarningsAndTTR";
    ttr.Put("methodName", Parameter(&methodName));
    GbxParameters parameters = GbxParameters();
    ttr.Put("params", Parameter(&parameters));
    currentCalls.push_back(ttr);

    for(int callId = 0; callId < currentCalls.size(); callId++)
    {
        structArray.Put(&(currentCalls.at(callId)));
    }
    params.Put(&structArray);

    GbxResponse response = query(GbxMessage("system.multicall", params));
    currentCalls = std::vector<GbxStructParameters>();
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

    //std::string gzipped = GZip::Compress(postfields.str());

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
    curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip");
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
    if(responseParams.size() != 0)
    {
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
    }

    curl_slist_free_all(headers);

    return response;
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
