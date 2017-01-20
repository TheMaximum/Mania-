#include "Config.h"

Config::Config(std::string configFile)
    : configFile(configFile)
{
    parseConfig();
}

Config::~Config()
{
    delete Server; Server = NULL;
    delete Database; Database = NULL;
    delete Program; Program = NULL;
    delete Plugins; Plugins = NULL;
}

void Config::parseConfig()
{
    std::cout << "[          ] Loading configuration file '" << configFile << "' ... " << '\r' << std::flush;
    YAML::Node config = YAML::LoadFile(configFile);
    YAML::Node server = config["config"]["server"];
    YAML::Node database = config["config"]["database"];
    YAML::Node program = config["config"]["program"];
    YAML::Node plugins = config["plugins"];
    std::cout << "[    \033[0;32mOK\033[0;0m" << std::endl;

    Server = new ServerConfig();
    Server->address = server["address"].as<std::string>();
    Server->port = server["port"].as<int>();
    Server->username = server["authentication"]["username"].as<std::string>();
    Server->password = server["authentication"]["password"].as<std::string>();

    Database = new DatabaseConfig();
    Database->address = database["address"].as<std::string>();
    Database->port = database["port"].as<int>();
    Database->username = database["authentication"]["username"].as<std::string>();
    Database->password = database["authentication"]["password"].as<std::string>();
    Database->database = database["database"].as<std::string>();

    Program = new ProgramConfig();
    Program->checkVersion = program["checkVersion"].as<bool>();

    parsePlugins(plugins);
}

void Config::parsePlugins(YAML::Node plugins)
{
    Plugins = new std::map<std::string, PluginConfig>();

    for(int pluginId = 0; pluginId < plugins.size(); pluginId++)
    {
        PluginConfig pluginConfig = PluginConfig();
        pluginConfig.settings = std::map<std::string, std::string>();

        YAML::Node plugin = plugins[pluginId];
        if(plugin.Type() == YAML::NodeType::Scalar)
        {
            pluginConfig.name = plugin.as<std::string>();
        }
        else
        {
            for (YAML::const_iterator pluginIt = plugin.begin(); pluginIt != plugin.end(); ++pluginIt)
            {
                pluginConfig.name = pluginIt->first.as<std::string>();
                for (YAML::const_iterator settingIt = pluginIt->second.begin(); settingIt != pluginIt->second.end(); ++settingIt)
                {
                    pluginConfig.settings.insert(std::pair<std::string, std::string>(settingIt->first.as<std::string>(), settingIt->second.as<std::string>()));
                }
            }
        }

        Plugins->insert(std::pair<std::string, PluginConfig>(pluginConfig.name, pluginConfig));
    }
}
