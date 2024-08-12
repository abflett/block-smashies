cmake_minimum_required(VERSION 3.28)
project(BlockSmashies VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SourceFiles
    src/main.c
    src/scene_manager.c
    src/main_menu.c
    src/gameplay.c
    src/title_screen.c
    src/logo_screen.c
    src/game_settings.c
)

# Include directories
include_directories(include)

# Add raylib as a subdirectory
add_subdirectory(external/raylib)
add_subdirectory(external/parson)

# Create the BlockSmashies executable
add_executable(BlockSmashies ${SourceFiles})

# Link with raylib
target_link_libraries(BlockSmashies PUBLIC raylib)
target_link_libraries(BlockSmashies PUBLIC parson)

# Set build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)

# MSVC-specific settings
if (MSVC)
    # Use Multi-Processor Compilation
    add_compile_options(/MP)

    # Set MSVC-specific flags for each build type
    target_compile_options(BlockSmashies PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>           # Disable optimization, include debug info
        $<$<CONFIG:Release>:/O2 /DNDEBUG>    # Full optimization, no debug info
        $<$<CONFIG:MinSizeRel>:/O1>          # Optimize for size
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>  # Full optimization, include debug info
    )

    # Warnings as errors for stricter code (optional)
    target_compile_options(BlockSmashies PRIVATE /W3 /WX)
    
    # Disable specific MSVC warnings (optional)
    target_compile_options(BlockSmashies PRIVATE /wd4996)
else()
    # GCC/Clang-specific flags
    target_compile_options(BlockSmashies PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:MinSizeRel>:-Os>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
    )
endif()

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "Setting build type to 'Debug' as default.")
endif()
