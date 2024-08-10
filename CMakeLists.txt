cmake_minimum_required(VERSION 3.28)
project(BlockSmashies VERSION 0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SourceFiles src/main.c)
include_directories(include)

find_package(raylib REQUIRED)

add_executable(BlockSmashies ${SourceFiles})

target_link_libraries(BlockSmashies ${raylib_LIBRARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
add_compile_options(-Wall -Wextra -g)