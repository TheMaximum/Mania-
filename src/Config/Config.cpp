#include "Config.h"

Config::Config(std::string configFile)
    : configFile(configFile)
{
    parseConfig();
}

Config::~Config()
{
    delete Server;
    Server = NULL;
}

void Config::parseConfig()
{
    std::cout << "[         ] Loading configuration file '" << configFile << "' ... " << '\r' << std::flush;
    YAML::Node config = YAML::LoadFile(configFile);
    YAML::Node server = config["config"]["server"];
    std::cout << "[   \033[0;32mOK.\033[0;0m" << std::endl;

    Server = new ServerConfig();
    Server->address = server["address"].as<std::string>();
    Server->port = server["port"].as<int>();
    Server->username = server["authentication"]["username"].as<std::string>();
    Server->password = server["authentication"]["password"].as<std::string>();
}
