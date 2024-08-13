cmake_minimum_required(VERSION 3.28)
project(BlockSmashies VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add third-party subdirectories
add_subdirectory(external/raylib)
add_subdirectory(external/parson)

# Include directories
include_directories(
    "include/core"
    "include/scenes"
    "include/game_states"
    "external/parson"
)

# Add source files for the core module
set(CORE_SRC
    src/core/main.c
    src/core/scene_manager.c
    src/core/game_settings.c
    src/core/high_score.c
)

# Add source files for scenes
set(SCENES_SRC
    src/scenes/main_menu.c
    src/scenes/gameplay.c
    src/scenes/title_screen.c
    src/scenes/logo_scene.c
)

# Add source files for game states
set(GAME_STATES_SRC
    src/game_states/play_state_manager.c
    src/game_states/playing_menu_state.c
    src/game_states/playing_state.c
)

# Create the BlockSmashies executable
add_executable(BlockSmashies
    ${SCENES_SRC}
    ${GAME_STATES_SRC}
    ${CORE_SRC}
    )


# Link with raylib
target_link_libraries(BlockSmashies PUBLIC raylib)
target_link_libraries(BlockSmashies PUBLIC parson)

# Custom command to copy assets directory to the build directory
add_custom_command(TARGET BlockSmashies POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:BlockSmashies>/assets
)

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
