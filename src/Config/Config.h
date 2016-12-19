#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <iostream>

#include <yaml-cpp/yaml.h>

struct ServerConfig
{
    std::string address;
    int port;
    std::string username;
    std::string password;
};

class Config
{
public:
    Config(std::string configFile);

    ServerConfig* Server;

private:
    void parseConfig();
    std::string configFile;
};

#endif // CONFIG_H_
