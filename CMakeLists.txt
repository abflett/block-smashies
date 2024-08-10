cmake_minimum_required(VERSION 3.28)
project(BlockSmashies VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SourceFiles src/main.c)
include_directories(include)

add_subdirectory(external/raylib)

add_executable(BlockSmashies ${SourceFiles})

target_link_libraries(BlockSmashies PUBLIC raylib)