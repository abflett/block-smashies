#include "raylib.h"
#include "pause_menu_state.h"
#include "game_state_manager.h"
#include "playing_state.h"
#include "scene_manager.h"
#include "logo_scene.h"
#include "game_settings.h"



void pause_menu_state_init(void)
{
}

void pause_menu_state_update(float delta_time)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (playing_state.cleanup)
        {
            game_settings.is_paused = false;
            playing_state.cleanup();
        }

        scene_manager.next_scene = &logo_scene;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        game_state_manager.next_state = &playing_state;
    }
}

void pause_menu_state_render(void)
{
    ClearBackground(BLACK);
    DrawText("Menu, Escape to exit or Enter to continue playing!", 5, 10, 8, LIGHTGRAY);
}

void pause_menu_state_cleanup(void)
{
}

GameState pause_menu_state = {
    .init = pause_menu_state_init,
    .update = pause_menu_state_update,
    .render = pause_menu_state_render,
    .cleanup = pause_menu_state_cleanup,
};