cmake_minimum_required(VERSION 3.28)
project(BlockSmashies VERSION 0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SourceFiles src/main.c)

add_executable(BlockSmashies ${SourceFiles})
