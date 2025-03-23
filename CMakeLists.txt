cmake_minimum_required(VERSION 3.30)
project(SnakeAI)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(SnakeAI src/main.cpp)
target_link_libraries(SnakeAI PUBLIC raylib)
