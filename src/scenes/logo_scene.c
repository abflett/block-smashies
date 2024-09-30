#include "raylib.h"
#include "logo_scene.h"
#include "settings.h"
#include "scene_manager.h"

static Scene logo_scene;

static float elapsed_time = 0.0f; // Variable to track elapsed time

static void scene_init(int arg_count, va_list args)
{
    elapsed_time = 0.0f;
}

static void scene_update(float delta_time)
{
    elapsed_time += delta_time; // Increment elapsed time by delta_time

    if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP) || elapsed_time >= settings.game.logo_screen_time)
    {
        scene_manager.change(scene_manager.scenes.title, 0);
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

Scene *create_logo_scene()
{
    logo_scene.init = scene_init;
    logo_scene.update = scene_update;
    logo_scene.render = scene_render;
    logo_scene.cleanup = scene_cleanup;

    return &logo_scene;
};