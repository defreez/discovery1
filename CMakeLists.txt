cmake_minimum_required(VERSION 3.20)
project(discovery1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Main executable
add_executable(discovery1
    src/main.cpp
    src/ship.cpp
    src/hal.cpp
)

target_include_directories(discovery1 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(discovery1 PRIVATE raylib)

# Assets directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
