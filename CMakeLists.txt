cmake_minimum_required(VERSION 3.13)

project(bittorrent-starter-cpp)

file(GLOB SOURCE_FILES 
    src/*.cpp
    src/*.hpp
    src/lib/*/*.cpp
    src/commands/*.cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(bittorrent ${SOURCE_FILES})

target_include_directories(bittorrent PUBLIC src)