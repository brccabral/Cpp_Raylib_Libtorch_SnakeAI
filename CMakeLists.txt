cmake_minimum_required(VERSION 3.28)
project(SnakeAI)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)
find_package(Torch REQUIRED)

file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(SnakeAI ${CPP_FILES})
target_link_libraries(SnakeAI PUBLIC raylib ${TORCH_LIBRARIES})

add_custom_command(TARGET SnakeAI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/models
)
