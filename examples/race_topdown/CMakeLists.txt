cmake_minimum_required(VERSION 3.27)
project(RaceTopDown)
function (print_cmake_variables_matching pattern)
    get_cmake_property(_variableNames VARIABLES)
    string(TOLOWER "${pattern}" pattern_lower)
    foreach (_variableName ${_variableNames})
        string(TOLOWER "${_variableName}" variable_lower)
        if (variable_lower MATCHES "${pattern_lower}")
            message(STATUS "${_variableName}=${${_variableName}}")
        endif ()
    endforeach ()
endfunction ()

find_package(raylib REQUIRED)
print_cmake_variables_matching("raylib")
# message(FATAL_ERROR "block")

file(GLOB CPP_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${CPP_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC raylib mlgames plotter::plotter)

if (MSVC)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/models
    )
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets
    )
else ()
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/models
    )
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/assets
    )
endif ()
