cmake_minimum_required(VERSION 3.28)
project(BlockSmashies VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SourceFiles
    src/main.c
    src/scene_manager.c
    src/main_menu.c
    src/gameplay.c
    src/title_screen.c
    src/logo_screen.c
    src/game_settings.c
)

include_directories(include)

add_subdirectory(external/raylib)

add_executable(BlockSmashies ${SourceFiles})

target_link_libraries(BlockSmashies PUBLIC raylib)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
endif()