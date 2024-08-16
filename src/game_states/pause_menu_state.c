#include "raylib.h"
#include "pause_menu_state.h"
#include "game_state_manager.h"
#include "scene_manager.h"
#include "game_settings.h"

static void state_init(void)
{
}

static void state_update(float delta_time)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (game_state_manager.states.playing->cleanup)
        {
            game_settings.is_paused = false;
            game_state_manager.states.playing->cleanup();
        }

        scene_manager.next_scene = scene_manager.scenes.logo;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        game_state_manager.next_state = game_state_manager.states.playing;
    }
}

static void state_render(void)
{
    ClearBackground(BLACK);
    DrawText("Menu, Escape to exit or Enter to continue playing!", 5, 10, 8, LIGHTGRAY);
}

static void state_cleanup(void)
{
}

GameState pause_menu_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};