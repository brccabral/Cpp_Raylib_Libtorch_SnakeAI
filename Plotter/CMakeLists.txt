cmake_minimum_required(VERSION 3.23)
project(Plotter)

find_package(rlImGui REQUIRED)

add_library(plotter STATIC)
add_library(plotter::plotter ALIAS plotter)

file(GLOB CPP_FILES "*.cpp")
file(GLOB H_FILES "./include/*.h")

target_sources(
    plotter
    PUBLIC FILE_SET
           plotter_headers
           TYPE
           HEADERS
           BASE_DIRS
           ./include
           FILES
           ${H_FILES}
    PRIVATE ${CPP_FILES}
)

target_link_libraries(plotter PUBLIC rlImGui::rlImGui)
