#include "raylib.h"
#include "logo_scene.h"
#include "game_settings.h"
#include "scene_manager.h"
#include "title_scene.h"

static float elapsed_time = 0.0f; // Variable to track elapsed time

static void scene_init(void)
{
    elapsed_time = 0.0f;
}

static void scene_update(float delta_time)
{
    elapsed_time += delta_time; // Increment elapsed time by delta_time

    if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP) || elapsed_time >= game_settings.logo_screen_time)
    {
        scene_manager.next_scene = &title_scene;
    }
}

static void scene_render(void)
{
    DrawText("Logo Screen - (Slide show before the title screen)", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Title Screen", 5, 16, 8, WHITE);
}

static void scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene logo_scene = {
    .init = scene_init,
    .update = scene_update,
    .render = scene_render,
    .cleanup = scene_cleanup,
};