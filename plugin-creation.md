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
    HelloGoodbyePlugin* plugin = new HelloGoodbyePlugin();
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

### Plugin Init() ###
The ```Plugin``` interface demands you have an ```Init()``` method in your plugin class. This method is being called after all plugins have been loaded by the system.
At this moment all needed instances have been set for your plugin and can be called.

### Plugin CallBack methods ###
Almost all callbacks from the server are relayed to the plugins. The table indicates which callback can be received via which methods vector (on which to register your method) and which parameters you will receive in your method.

| CallBack                      | Methods vector    | Method parameters                                                          |
| ----------------------------- | ----------------- | -------------------------------------------------------------------------- |
| ManiaPlanet.PlayerConnect     | PlayerConnect     | ```Player```                                                               |
| ManiaPlanet.PlayerDisconnect  | PlayerDisconnect  | ```Player```                                                               |
| ManiaPlanet.PlayerChat        | PlayerChat        | ```Player```, ```std::string```, ```bool```                                |
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

### Calling server methods ###
Calling methods on the server can be easily done by using the ```Methods``` pointer included in your plugin (```methods->```).
Almost all server methods can be called via this class, check the full list [in Doxygen](https://themaximum.github.io/mania-pp/docs/develop/html/classMethods.html).
