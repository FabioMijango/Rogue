cmake_minimum_required(VERSION 4.2)
project(ReRogue)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
        SGEL
        GIT_REPOSITORY https://github.com/FabioMijango/SGEL.git
        # TODO: Change to a stable release tag when available
        GIT_TAG e239649faa651defb1cda1dc456236e2bcb4ebfa
        GIT_SHALLOW TRUE
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SGEL)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} SGEL)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)