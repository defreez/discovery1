cmake_minimum_required(VERSION 3.20)
project(discovery1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch dependencies
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(raylib json googletest)

# Core library (headless, no raylib)
add_library(discovery1_core STATIC
    src/core/world_loader.cpp
)
target_include_directories(discovery1_core PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(discovery1_core PUBLIC nlohmann_json::nlohmann_json)

# Main executable (with rendering)
add_executable(discovery1
    src/main.cpp
    src/platform/renderer.cpp
    src/platform/input.cpp
)
target_include_directories(discovery1 PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(discovery1 PRIVATE discovery1_core raylib)

# Copy maps to build directory
file(COPY ${CMAKE_SOURCE_DIR}/maps DESTINATION ${CMAKE_BINARY_DIR})

# Headless tests (Google Test)
enable_testing()
add_executable(test_core
    tests/test_math.cpp
    tests/test_player.cpp
    tests/test_world.cpp
    tests/test_world_loader.cpp
    tests/test_texture_gen.cpp
)
target_link_libraries(test_core PRIVATE discovery1_core GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test_core)
