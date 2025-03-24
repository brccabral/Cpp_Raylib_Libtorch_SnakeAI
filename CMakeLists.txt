cmake_minimum_required(VERSION 3.28)
project(SnakeAI)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(SnakeAI ${CPP_FILES})
target_link_libraries(SnakeAI PUBLIC raylib)
