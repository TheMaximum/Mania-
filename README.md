# Mania++
Mania++ is an in-development Server Controller for TrackMania 2 (ManiaPlanet) servers. It is written in C++ and is at this moment basically a hobby-project. You're free to try it and contribute to the system, all ideas and assitance is welcome. No guarantee this makes it to a release-ready state, so be aware of that before using it on your servers.

## Current build status ##
* Master: [![Build Status](https://travis-ci.org/TheMaximum/mania-pp.svg?branch=master)](https://travis-ci.org/TheMaximum/mania-pp)
* Develop: [![Build Status](https://travis-ci.org/TheMaximum/mania-pp.svg?branch=develop)](https://travis-ci.org/TheMaximum/mania-pp)

Also checkout the [Mania++ versioning system](https://themaximum.github.io/mania-pp/versioning.html) for more about the software versions and branches.

## License ##
Mania++ and its sources are available under the [GNU General Public License v3](https://www.gnu.org/licenses/#GPL). This allows users/contributors to make changes to the source code, as long as those changes are made available under the same license as the original. The copyright/license notices should not be removed from the sources/workspace.

## Tested environments ##
* Ubuntu 16.10 (Linux 4.8.0-30), 64-bit with gcc 6.2.0 (development)
* Ubuntu 16.04.1 LTS (Linux 4.4.0-31), 32-bit with gcc 5.4.0 (testing)
* Ubuntu 14.04.5 LTS (Linux 4.4.0-51), 64-bit with gcc 4.9.4 (Travis)
* Debian 8.6 (Linux 3.16.0-4), 64-bit with gcc 4.9.2 (production)
* "Bash on Ubuntu 14.04.5 LTS on Windows 10" (Linux 4.4.0-51), 64-bit with gcc 4.9.4 (development)

The ```./install.sh```-script requires a Debian-like system to function (with ```apt-get```). The JSON library requires gcc/g++ v4.9+, the 14.04 LTS only comes with 4.8.4, so you will have to [upgrade the version by hand](http://askubuntu.com/a/456849) and tell the build script that you want to use that version: ```-DCMAKE_CXX_COMPILER=g++-4.9 -DCMAKE_C_COMPILER=gcc-4.9```.

## Requirements ##
* Minimum gcc 4.9 / C++11
* Git
* [CMake](https://cmake.org)

## Aims and working points ##
* [Be comparable or better than standard PhpRemote](https://themaximum.github.io/mania-pp/comparison.html)

## Achieved goals ##
* Working GbxRemote module
   * Send methods and receive responses
   * Receive callbacks
   * De-XMLify responses and callbacks
* Easy-to-use objects (f.e. `Player` and `Map`)
* Working configuration system (YAML)
* Database support (MySQL)
* ManiaLink support
   * ManiaLink handler (`UIManager`)
   * Easy use of a ManiaLink list (`UIList`)
* Chat command handling
* Working plugin system
   * Plugin interface
   * Callback handling
   * Access to server, database, playerlist, maplist and ManiaLink
   * Chat commands
   * Receive settings from configuration file
   * _Welcome/goodbye, maplist/jukebox, local records, map karma and mapinfo widget_

## Installing for the first time ##
* ```./install.sh```
    * Installs Boost (requirement for yaml-cpp)
    * Installs [yaml-cpp](https://github.com/jbeder/yaml-cpp/) 0.5.3
    * Installs [pugixml](https://github.com/zeux/pugixml) 1.8.1
    * Installs [JSON for Modern C++](https://github.com/nlohmann/json) 2.0.10

## Building ##
* ```./build.sh```
* Move ```config.dist.yaml``` to ```config.yaml```
* Edit the configuration file with the server/database information

## Running ##
* ```./Mania++```

## Check documentation of latest develop commit ##
* Latest master: [Doxygen HTML](https://themaximum.github.io/mania-pp/docs/master/html/)
* Latest develop: [Doxygen HTML](https://themaximum.github.io/mania-pp/docs/develop/html/)

## Generate code documentation for current code ##
_Requires Doxygen to be installed on your system: ```sudo apt-get install doxygen```_

To make it easier to contribute, you can check out the (automatic) documentation.
To do this, do:

* ```cd ./doc/```
* ```doxygen```
* Open the documentation either in HTML or LaTeX format (in subdirectories)
