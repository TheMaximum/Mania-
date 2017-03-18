cmake_minimum_required (VERSION 2.6)
project (Mania++)

add_definitions(-std=c++11)
add_definitions(-Wno-deprecated)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib/yaml/build)
include_directories("${PROJECT_SOURCE_DIR}/lib/yaml/include")
include_directories("${PROJECT_SOURCE_DIR}/lib/json")
include_directories("${PROJECT_SOURCE_DIR}/lib/base64")

set (PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)
set (VERSION_PATCH 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/Config/Version.h.in"
    "${PROJECT_BINARY_DIR}/Config/Version.h"
)

include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES src/*.cpp)

find_package(Boost REQUIRED COMPONENTS iostreams regex)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Mania++ ${SOURCES}
                       lib/pugixml/libpugixml.a
                       lib/base64/base64.cpp)
target_link_libraries(Mania++ yaml-cpp
                              ${EXECUTABLE_OUTPUT_PATH}/lib/pugixml/libpugixml.a
                              dl
                              mysqlcppconn
                              curl
                              ${Boost_LIBRARIES})
