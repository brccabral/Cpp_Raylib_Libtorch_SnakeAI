cmake_minimum_required(VERSION 3.27)
project(MLGames)

add_subdirectory(mlgames)

option(BUILD_EXAMPLES "Build examples" ON)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
