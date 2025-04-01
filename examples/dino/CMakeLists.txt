cmake_minimum_required(VERSION 3.27)
project(Dino)

find_package(raylib REQUIRED)

file(GLOB CPP_FILES "*.cpp")
add_executable(Dino ${CPP_FILES})
target_link_libraries(Dino PUBLIC raylib mlgames)

if (MSVC)
    add_custom_command(TARGET Dino POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/models
    )
else ()
    add_custom_command(TARGET Dino POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/models
    )
endif ()

