cmake_minimum_required(VERSION 3.28)
project(Rogue LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp
        src/core/Game.cpp
        src/core/Game.hpp
        src/core/ResourceManager.cpp
        src/core/ResourceManager.hpp
        src/bible/Bible.hpp
        src/core/Random.hpp
        src/core/Dungeon.cpp
        src/core/Dungeon.hpp
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)

add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:main>/res"
)

set_target_properties(main PROPERTIES
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)