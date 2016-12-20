#include "Config.h"

Config::Config(std::string configFile)
    : configFile(configFile)
{
    parseConfig();
}

void Config::parseConfig()
{
    std::cout << "Loading configuration file '" << configFile << "' ... ";
    YAML::Node config = YAML::LoadFile(configFile);
    std::cout << "OK" << std::endl;
    YAML::Node server = config["config"]["server"];
    std::cout << "Server NODE" << std::endl;

    Server = new ServerConfig();
    Server->address = server["address"].as<std::string>();
    Server->port = server["port"].as<int>();
    Server->username = server["authentication"]["username"].as<std::string>();
    Server->password = server["authentication"]["password"].as<std::string>();
}
