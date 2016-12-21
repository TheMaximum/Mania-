cmake_minimum_required (VERSION 2.6)
project (Mania++)

add_definitions(-std=c++11)
add_definitions(-Wno-deprecated)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib/yaml/build)
link_directories(${PROJECT_SOURCE_DIR}/lib/tinyxml)
include_directories("${PROJECT_SOURCE_DIR}/lib/yaml/include")
include_directories("${PROJECT_SOURCE_DIR}/lib/tinyxml")

set (PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 0)
set (VERSION_PATCH 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/Config/Version.h.in"
    "${PROJECT_BINARY_DIR}/Config/Version.h"
)

include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(Mania++ ${SOURCES})
target_link_libraries(Mania++ yaml-cpp tinyxml2)