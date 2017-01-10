#include "VersionChecker.h"

bool VersionChecker::CheckForUpdates(std::string repository, std::string currentVersion)
{
    std::cout << "[         ] Retrieving versions for '" << repository << "' ... " << '\r' << std::flush;
    std::string rateLimit;
    std::string url = "https://api.github.com/rate_limit";
    retrieveData(url, &rateLimit);
    json j = json::parse(rateLimit);
    json rate = j.at("rate");

    std::stringstream limitStr;
    limitStr << rate.at("limit");
    int limit = atoi(limitStr.str().c_str());
    std::stringstream remainingStr;
    remainingStr << rate.at("remaining");
    int remaining = atoi(remainingStr.str().c_str());
    std::stringstream resetStr;
    resetStr << rate.at("reset");
    int reset = atoi(resetStr.str().c_str());

    if(remaining == 0)
    {
        Logging::PrintFailedFlush();
        std::time_t currentTime = std::time(0);
        int timeDifference = reset - currentTime;
        int minutesDifference = floor(timeDifference / 60);
        int secondsDifference = timeDifference - (minutesDifference * 60);

        std::stringstream rateError;
        rateError << "Hit Github rate limiter (" << limit << "), reset in " << minutesDifference << ":" << secondsDifference << ".";
        Logging::PrintError(-1, rateError.str());
    }
    else
    {
        std::vector<GitVersion> versions = retrieveVersions(repository);
        Logging::PrintOKFlush();

        std::vector<GitVersion> newVersions = std::vector<GitVersion>();
        for(int versionId = 0; versionId < versions.size(); versionId++)
        {
            GitVersion version = versions.at(versionId);
            version.Repository = repository;
            bool newer = VersionCompare::NewerThanCurrent(currentVersion, version.Tag);
            if(newer)
            {
                retrieveRelease(&version);
                newVersions.push_back(version);
            }

            /*std::cout << "Repository : " << version.Repository << std::endl;
            std::cout << "Tag        : " << version.Tag << std::endl;
            std::cout << "Name       : " << version.Name << std::endl;
            std::cout << "Commit     : " << version.Commit << std::endl;
            std::cout << "PreRelease : " << version.PreRelease << std::endl;
            std::cout << "Newer      : " << newer << std::endl;*/
        }

        if(newVersions.size() > 0)
        {
            std::cout << "[ VERSION ] Got " << newVersions.size() << " new versions available for '" << repository << "':" << std::endl;
            for(int newVersionId = 0; newVersionId < newVersions.size(); newVersionId++)
            {
                GitVersion version = newVersions.at(newVersionId);
                std::cout << "[   NEW   ]   " << version.Name << " (" << version.Tag << ")";
                if(version.PreRelease)
                    std::cout << " (pre-release)";
                std::cout << std::endl;
            }
        }
        else
        {
            std::cout << "[ VERSION ] No new version available for '" << repository << "'." << std::endl;
        }
    }
}

size_t VersionChecker::receiveData(void* contents, size_t size, size_t nmemb, std::string* s)
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

void VersionChecker::retrieveData(std::string url, std::string* data)
{
    CURL* curl;
    CURLcode result;
    std::stringstream userAgent;
    userAgent << "Mania++/" << VERSION;
    std::string tags;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.str().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, VersionChecker::receiveData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

    result = curl_easy_perform(curl);
    if(result != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
}

std::vector<GitVersion> VersionChecker::retrieveVersions(std::string repository)
{
    std::string versions;
    std::string url = "https://api.github.com/repos/" + repository + "/tags";
    retrieveData(url, &versions);
    return decodeVersions(versions);
}

std::vector<GitVersion> VersionChecker::decodeVersions(std::string response)
{
    std::vector<GitVersion> versions = std::vector<GitVersion>();

    json j = json::parse(response);

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        json jsonName = it.value()["name"];
        std::string name = jsonName.get<std::string>();
        json jsonCommit = it.value()["commit"]["sha"];
        std::string commit = jsonCommit.get<std::string>();

        GitVersion version = GitVersion();
        version.Tag = name;
        version.Commit = commit;
        versions.push_back(version);
    }

    return versions;
}

void VersionChecker::retrieveRelease(GitVersion* version)
{
    std::string release;
    std::string url = "https://api.github.com/repos/" + version->Repository + "/releases/tags/" + version->Tag;
    retrieveData(url, &release);
    return decodeRelease(version, release);
}

void VersionChecker::decodeRelease(GitVersion* version, std::string response)
{
    json j = json::parse(response);

    json::iterator nameIt = j.find("name");
    if(nameIt != j.end())
    {
        json jsonName = nameIt.value();
        version->Name = jsonName.get<std::string>();
    }

    json::iterator prereleaseIt = j.find("prerelease");
    if(prereleaseIt != j.end())
    {
        json jsonPrerelease = prereleaseIt.value();
        version->PreRelease = jsonPrerelease.get<bool>();
    }
}
