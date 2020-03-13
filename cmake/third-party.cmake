include(ExternalProject)

set(EXTERNAL_PATH "${PROJECT_SOURCE_DIR}/.external" CACHE PATH "external deps path")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/external)

include(github/repos)

include(asio)
include(protobuf)

# gtest and benchmark
if(BUILD_RAFT_TEST)
    include(gtest)
endif()

# third-party header only
include_directories(${PROJECT_SOURCE_DIR}/third-party/spdlog-1.3.1/include)
