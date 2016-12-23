# Mania++
Mania++ is an in-development Server Controller for TrackMania 2 (ManiaPlanet) servers. It is written in C++ and is at this moment basically a hobby-project. You're free to try it and contribute to the system, all ideas and assitance is welcome. No guarantee this makes it to a release-ready state, so be aware of that before using it on your servers.

## License ##
Mania++ and its sources are available under the [GNU General Public License v3](https://www.gnu.org/licenses/#GPL). This allows users/contributors to make changes to the source code, as long as those changes are made available under the same license as the original. The copyright/license notices should not be removed from the sources/workspace.

## Current build status ##
* Master: [![Build Status](https://travis-ci.org/TheMaximum/mania-pp.svg?branch=master)](https://travis-ci.org/TheMaximum/mania-pp)
* Develop: [![Build Status](https://travis-ci.org/TheMaximum/mania-pp.svg?branch=develop)](https://travis-ci.org/TheMaximum/mania-pp)

Also checkout the [Mania++ versioning system](versioning.html) for more about the software versions and branches.

## Requirements ##
* C++11
* Git
* [CMake](https://cmake.org)

## Aims and working points ##
* Working plugin system
   * Plugin interface
   * Callback handling
   * Access to server, database and ManiaLink
* Database support
* ManiaLink support

## Achieved goals ##
* Working GbxRemote module
   * Send methods and receive responses
   * Receive callbacks
   * De-XMLify responses and callbacks
   * Create usable objects (f.e. `Player` or `Map`)
* Working configuration system (YAML)

## Installing for the first time ##
* ```./install.sh```
    * Installs Boost (requirement for yaml-cpp)
    * Installs [yaml-cpp](https://github.com/jbeder/yaml-cpp/) 0.5.3
    * Installs [pugixml](https://github.com/zeux/pugixml) 1.8.1
    * Builds the code (via ```build.sh```)

## Building (updates) ##
* ```./build.sh```
* Move ```config.dist.yaml``` to ```config.yaml```
* Edit the configuration file with the server information

## Running ##
* ```./Mania++```

## Check documentation of latest develop commit ##
* Latest master: not yet available
* Latest develop: [Doxygen HTML](https://themaximum.github.io/mania-pp/docs/develop/html/)

## Generate code documentation for current code ##
_Requires Doxygen to be installed on your system: ```sudo apt-get install doxygen```_

To make it easier to contribute, you can check out the (automatic) documentation.
To do this, do:

* ```cd ./doc/```
* ```doxygen```
* Open the documentation either in HTML or LaTeX format (in subdirectories)
