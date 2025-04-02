cmake_minimum_required(VERSION 3.27)
project(PegSolitaireAI)

find_package(raylib REQUIRED)

file(GLOB CPP_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${CPP_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC raylib mlgames)

if (MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/models
    )
else ()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/models
    )
endif ()
