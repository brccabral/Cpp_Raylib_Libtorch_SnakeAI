cmake_minimum_required(VERSION 3.27)
project(MLGamesLib)

find_package(Torch REQUIRED)

add_library(mlgames STATIC)
add_library(mlgames::mlgames ALIAS mlgames)

file(GLOB CPP_FILES "*.cpp")
file(GLOB H_FILES "./include/*.h")

target_sources(mlgames
        PUBLIC
        FILE_SET mlgames_headers
        TYPE HEADERS
        BASE_DIRS ./include
        FILES ${H_FILES}
        PRIVATE ${CPP_FILES}
)

target_compile_features(mlgames PUBLIC cxx_std_20)

target_link_libraries(mlgames PUBLIC ${TORCH_LIBRARIES})
