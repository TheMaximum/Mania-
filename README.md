# Mania++
* Master: [![Build Status](https://travis-ci.org/TheMaximum/mania-pp.svg?branch=master)](https://travis-ci.org/TheMaximum/mania-pp)
* Develop: [![Build Status](https://travis-ci.org/TheMaximum/mania-pp.svg?branch=develop)](https://travis-ci.org/TheMaximum/mania-pp)

## Requirements ##
* C++11
* Git
* [CMake](https://cmake.org)

## Installing for the first time ##
* ```./install.sh```
    * Installs Boost (requirement for yaml-cpp)
    * Installs [yaml-cpp](https://github.com/jbeder/yaml-cpp/) 0.5.3
    * Installs [TinyXML2](https://github.com/leethomason/tinyxml2/) 4.0.1
    * Builds the code

## Building (updates) ##
* ```./build.sh```
* Move ```config.dist.yaml``` to ```config.yaml```
* Edit the configuration file with the server information

## Running ##
* ```./Mania++```

## Get code documentation for development ##
_Requires Doxygen to be installed on your system: ```sudo apt-get install doxygen```_

To make it easier to contribute, you can check out the (automatic) documentation.
To do this, do:
* ```cd ./doc/```
* ```doxygen```
* Open the documentation either in HTML or LaTeX format (in subdirectories)
