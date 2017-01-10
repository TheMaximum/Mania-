cmake_minimum_required (VERSION 2.6)
project (Mania++)

add_definitions(-std=c++11)
add_definitions(-Wno-deprecated)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib/yaml/build)
include_directories("${PROJECT_SOURCE_DIR}/lib/yaml/include")
include_directories("${PROJECT_SOURCE_DIR}/lib/json")

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

add_executable(Mania++ ${SOURCES}
                       lib/pugixml/libpugixml.a)
target_link_libraries(Mania++ yaml-cpp
                              ${EXECUTABLE_OUTPUT_PATH}/lib/pugixml/libpugixml.a
                              dl
                              mysqlcppconn
                              curl)
