cmake_minimum_required (VERSION 2.6)
project (LocalRecordsPlugin)

add_definitions(-std=c++11)
add_definitions(-Wno-deprecated)
add_definitions(-Wl,--export-dynamic)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/../../lib/yaml/build)
include_directories("${PROJECT_SOURCE_DIR}/../../lib/yaml/include")
set (PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/../../../src")

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(LocalRecordsPlugin SHARED ${SOURCES})
target_link_libraries(LocalRecordsPlugin yaml-cpp mysqlcppconn)
