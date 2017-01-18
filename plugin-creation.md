---
title: Plugin Creation | Mania++
layout: "page"
---
# Plugin Creation #
Creating plugins for Mania++ is quite easy, most of the work of communicating with the controller is already available in the Plugin "interface".
With simple calls, you can add your functions to catch callbacks and call methods on the server. An example of a "complete", but small Plugin can be found [on the repository](https://github.com/TheMaximum/mania-pp/tree/develop/plugins/HelloGoodbye).

## main.cpp ##
The ```main.cpp``` file contains just one simple function, namely 'startPlugin':

```C++
extern "C" Plugin* startPlugin()
{
    ExamplePlugin* plugin = new ExamplePlugin();
    return (Plugin*)plugin;
}
```

This function is called by the plugin manager to retrieve the plugin class, which is inherited of the class ```Plugin``` in the Mania++ package.

## Your plugin class ##
The plugin class is just the base point to which Mania++ can rely information.
You can structure your plugin the way you like, but make sure to follow a few restrictions/guidelines.
Be aware that your plugin should inherit ```Plugin```, which should be included via ```Plugins/Plugin.h```.

### Plugin constructor ###
The constructor should be setting the version and author variables of the ```Plugin``` interface:

```C++
  Version = "0.1.0";
  Author = "YOU";
```

The constructor is also responsible of adding the functions which should be called when a callback comes in from the server.
You simply add your function to one of the vectors which contains all your plugin's callbackhandlers.
Only functions added in the constructor are taken into account by the system. Adding functions afterwards will have no effect.
Because the ```std::function``` doesn't work nicely with class methods, you can do the following:

```C++
  PlayerConnect.push_back([this](Player player) { OnPlayerConnect(player); });
```

In this case, ```OnPlayerConnect``` is the method you want to get called when a ```ManiaPlanet.PlayerConnect``` callback gets fired.

Adding chat commands also only works in the constructor and is simliar to subscribing on events. You can call the ```RegisterCommand``` (for /xxxx) and ```RegisterAdminCommand``` (for /admin xxxx) methods:

```C++
RegisterCommand("test", [this](Player player, std::vector<std::string> parameters) { OnTestCommand(player, parameters); });
RegisterAdminCommand("test", [this](Player player, std::vector<std::string> parameters) { OnAdminTestCommand(player, parameters); });
```

The parameters you receive in your chat-function are the remaining words (so seperated by spaces) in the chat line.

### Plugin Init() ###
The ```Plugin``` interface demands you have an ```Init()``` method in your plugin class. This method is being called after all plugins have been loaded by the system.
At this moment all needed instances have been set for your plugin and can be called.

### Plugin CallBack methods ###
Almost all callbacks from the server are relayed to the plugins. The table indicates which callback can be received via which methods vector (on which to register your method) and which parameters you will receive in your method.

| CallBack                      | Methods vector    | Method parameters                                                          |
| ----------------------------- | ----------------- | -------------------------------------------------------------------------- |
| Every second (from controller) | EverySecond     |                         |
| Every minute (from controller) | EveryMinute     |                         |
| ManiaPlanet.PlayerConnect     | PlayerConnect     | ```Player```                                                               |
| ManiaPlanet.PlayerDisconnect  | PlayerDisconnect  | ```Player```                                                               |
| ManiaPlanet.PlayerChat        | PlayerChat        | ```Player```, ```std::string```                |
| ManiaPlanet.Echo              | Echo              | ```std::string```, ```std::string```                                       |
| ManiaPlanet.BeginMatch        | BeginMatch        |                                                                            |
| ManiaPlanet.EndMatch          | EndMatch          | ```std::vector<PlayerRanking>```, ```int```                                |
| ManiaPlanet.BeginMap          | BeginMap          | ```Map```                                                                  |
| ManiaPlanet.EndMap            | EndMap            | ```Map```                                                                  |
| ManiaPlanet.StatusChanged     | StatusChanged     | ```int```, ```std::string```                                               |
| TrackMania.PlayerCheckpoint   | PlayerCheckpoint  | ```Player```, ```int```, ```int```, ```int```                              |
| TrackMania.PlayerFinish       | PlayerFinish      | ```Player```, ```int```                                                    |
| TrackMania.PlayerIncoherence  | PlayerIncoherence | ```Player```                                                               |
| ManiaPlanet.BillUpdated       | BillUpdated       | ```int```, ```int```, ```std::string```, ```int```                         |
| ManiaPlanet.MapListModified   | MapListModified   | ```int```, ```int```, ```bool```                                           |
| ManiaPlanet.PlayerInfoChanged | PlayerInfoChanged | ```Player```                                                               |
| ManiaPlanet.VoteUpdated       | VoteUpdated       | ```std::string```, ```std::string```, ```std::string```, ```std::string``` |

```ManiaPlanet.PlayerManialinkPageAnswer``` callbacks are only available via the ```UIManager```.

### Accessing controller features ###
The controller features are accessible via a ```Controller``` struct, which is included in your plugin as ```controller->```. From here, you can execute queries on the server, access the current playerlist and maplist (and make changes in them) and query the database.

| Controller feature | Variable    | Documentation |
| ------------------ | ----------- | ------------- |
| Querying dedicated server | ```Server``` | [Methods](https://themaximum.github.io/mania-pp/docs/develop/html/classMethods.html) |
| Current player list | ```Players``` | Simple vector with [Player](https://themaximum.github.io/mania-pp/docs/develop/html/structPlayer.html) objects |
| Current map list | ```Maps``` | [MapList](https://themaximum.github.io/mania-pp/docs/develop/html/classMapList.html) |
| Database access | ```Database``` | [MySQL C++ Connector](https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-getting-started-examples.html) |
| Interface Manager | ```UI``` | [UIManager](https://themaximum.github.io/mania-pp/docs/develop/html/classUIManager.html) |
| Inter-plugin communication | ```Plugins``` | [PluginHandler](https://themaximum.github.io/mania-pp/docs/develop/html/classPluginHandler.html) |
| Server information | ```Info``` | [ServerInfo](https://themaximum.github.io/mania-pp/docs/develop/html/structServerInfo.html) |
