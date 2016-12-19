#include "Config.h"

Config::Config(std::string configFile)
    : configFile(configFile)
{
    parseConfig();
}

void Config::parseConfig()
{
    YAML::Node config = YAML::LoadFile(configFile);
    YAML::Node server = config["config"]["server"];

    Server = new ServerConfig();
    Server->address = server["address"].as<std::string>();
    Server->port = server["port"].as<int>();
    Server->username = server["authentication"]["username"].as<std::string>();
    Server->password = server["authentication"]["password"].as<std::string>();
}
