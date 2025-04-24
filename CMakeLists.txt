cmake_minimum_required(VERSION 3.27)
project(MLGames)

option(BUILD_EXAMPLES "Build examples" ON)

add_subdirectory(mlgames)
add_subdirectory(Plotter)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
