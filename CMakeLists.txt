cmake_minimum_required(VERSION 3.13)

project(bittorrent-starter-cpp)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

file(GLOB COMMON_SOURCES
    src/lib/*/*.hpp
    src/lib/*/*.cpp)

file(GLOB TARGET_SOURCES 
    src/*.hpp
    src/*.cpp
    src/commands/*.cpp)

file(GLOB TEST_SOURCES
    src/tests/*.hpp
    src/tests/*.cpp)

add_library(common SHARED ${COMMON_SOURCES})

add_executable(bittorrent ${TARGET_SOURCES})
target_link_libraries(bittorrent PRIVATE common)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE common)

target_include_directories(common PUBLIC src)
target_include_directories(bittorrent PUBLIC src)
target_include_directories(tests PUBLIC src)

add_custom_target(
    test
    DEPENDS tests
    COMMAND ./tests
    COMMENT "\n\nTesting"
)