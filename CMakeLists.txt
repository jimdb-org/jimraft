cmake_minimum_required(VERSION 2.6)
project(jim-raft CXX)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

set(CXX_FLAGS
    -std=c++11
    -g
    -Wall
    -Wextra
    -Wformat
    -Wsign-compare
    -Wno-unused-parameter
    -Wno-unused-variable
    -Woverloaded-virtual
    -Wno-missing-field-initializers
    -D__STDC_FORMAT_MACROS
    -DASIO_STANDALONE
)

string(REPLACE ";" " " CXX_FLAGS_TEMP "${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS_TEMP}")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")
endif()

MESSAGE(STATUS CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

if (APPLE)
    set(CMAKE_MACOSX_RPATH 1)
endif()

add_custom_target(build-3rd)
include(cmake/third-party.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# gcc address sanitize
OPTION (ENABLE_SANITIZE "Use gcc address sanitize" OFF)
MESSAGE(STATUS ENABLE_SANITIZE=${ENABLE_SANITIZE})
if(ENABLE_SANITIZE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/raft/include)

add_subdirectory(base)
add_subdirectory(common)
add_subdirectory(net)
add_subdirectory(raft)
