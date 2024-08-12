#include "logo_screen.h"
#include "raylib.h"
#include "scene_manager.h"
#include "title_screen.h"
#include "game_settings.h"

extern SceneManager scene_manager;

Scene logo_screen_scene = {
    .init = logo_screen_init,
    .update = logo_screen_update,
    .render = logo_screen_render,
    .cleanup = logo_screen_cleanup,
};

static float elapsed_time = 0.0f; // Variable to track elapsed time

void logo_screen_init(void)
{
    elapsed_time = 0.0f;
}

void logo_screen_update(float delta_time)
{
    elapsed_time += delta_time; // Increment elapsed time by delta_time

    if (elapsed_time >= game_settings.logo_screen_time) // Check if 2 seconds have passed
    {
        scene_manager.next_scene = &title_screen_scene; // Schedule transition to main_menu_scene
    }

    if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP))
    {
        scene_manager.next_scene = &title_screen_scene;
    }
}

void logo_screen_render(void)
{
    DrawText("Logo Screen - (Slide show before the title screen)", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Title Screen", 5, 16, 8, WHITE);
}

void logo_screen_cleanup(void)
{
    // Cleanup resources used by the main menu
}