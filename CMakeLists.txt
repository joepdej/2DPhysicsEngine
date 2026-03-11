cmake_minimum_required(VERSION 3.14)
project(2DPhysicsEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch raylib
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

# Fetch raylib-cpp
FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG v5.0.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib_cpp)

# Define the executable
add_executable(${PROJECT_NAME} src/body.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp)
