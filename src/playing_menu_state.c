#include "raylib.h"
#include "playing_menu_state.h"
#include "playing_state.h"
#include "play_state_manager.h"
#include "scene_manager.h"
#include "raylib.h"
#include "logo_screen.h"

extern PlayStateManager play_state_manager;
extern SceneManager scene_manager;

PlayState playing_menu_state = {
    .init = playing_menu_state_init,
    .update = playing_menu_state_update,
    .render = playing_menu_state_render,
    .cleanup = playing_menu_state_cleanup,
};

void playing_menu_state_init(void)
{
}

void playing_menu_state_update(float delta_time)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        scene_manager.next_scene = &logo_screen_scene;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        play_state_manager.next_state = &playing_state;
    }
}

void playing_menu_state_render(void)
{
    ClearBackground(BLACK);
    DrawText("Menu, Escape to exit or Enter to continue playing!", 5, 10, 8, LIGHTGRAY);
}

void playing_menu_state_cleanup(void)
{
}
