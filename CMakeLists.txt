cmake_minimum_required(VERSION 3.28)
project(BlockSmashies VERSION 0.1)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Third-party subdirectories
add_subdirectory(external/raylib)
add_subdirectory(external/parson)
add_subdirectory(external/box2d)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include/core
    ${PROJECT_SOURCE_DIR}/include/scenes
    ${PROJECT_SOURCE_DIR}/include/game_states
    ${PROJECT_SOURCE_DIR}/include/game
    ${PROJECT_SOURCE_DIR}/external/parson
    ${PROJECT_SOURCE_DIR}/external/uthash/src
    ${PROJECT_SOURCE_DIR}/external/klib
    ${PROJECT_SOURCE_DIR}/external/box2d/include
)

# Source files for the core module
set(CORE_SRC
    src/core/main.c
    src/core/game_settings.c
    src/core/high_score.c
    src/core/resource_manager.c
    src/core/ext_collision.c
)

# Source files for scenes
set(SCENES_SRC
    src/scenes/scene_manager.c
    src/scenes/main_menu_scene.c
    src/scenes/gameplay_scene.c
    src/scenes/title_scene.c
    src/scenes/logo_scene.c
)

# Source files for game states
set(GAME_STATES_SRC
    src/game_states/game_state_manager.c
    src/game_states/pause_menu_state.c
    src/game_states/playing_state.c
    src/game_states/game_over_state.c
)

# Source files for game
set(GAME_SRC
    src/game/player.c
    src/game/paddle.c
    src/game/ball.c
    src/game/brick.c
    src/game/game_status.c
    src/game/entities.c
    src/game/wall_edges.c
)

# Create the BlockSmashies executable
add_executable(BlockSmashies
    ${SCENES_SRC}
    ${GAME_STATES_SRC}
    ${CORE_SRC}
    ${GAME_SRC}
    )

# Link with libraries
target_link_libraries(BlockSmashies PUBLIC raylib parson box2d)

# Custom command to copy assets directory to the build directory
add_custom_command(TARGET BlockSmashies POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:BlockSmashies>/assets
)

# Set build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)

# compiler specific settings
if (MSVC)
    add_compile_options(/MP)
    target_compile_options(BlockSmashies PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2 /DNDEBUG>
        $<$<CONFIG:MinSizeRel>:/O1>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
    )
    target_compile_options(BlockSmashies PRIVATE /W3 /WX /wd4996)
else()
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