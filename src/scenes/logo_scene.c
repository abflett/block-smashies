#include "raylib.h"
#include "logo_scene.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"

static Scene logo_scene;
static float min_scene_time = 0.0f; // Variable to track elapsed time
static Font *font;

static void scene_init(int arg_count, va_list args)
{
    min_scene_time = 0.0f;
}

static void scene_update(float delta_time)
{
    min_scene_time += delta_time; // Increment elapsed time by delta_time

    if (GetKeyPressed() != 0 || GetGamepadButtonPressed() != 0 || IsGestureDetected(GESTURE_TAP) || min_scene_time >= settings.game.logo_screen_time)
    {
        scene_manager.change(scene_manager.scenes.title, 0);
    }
}

static void scene_render(void)
{
    const char *text = "Press any button";
    Vector2 text_size = MeasureTextEx(*font, text, 8, 0.0f);
    Vector2 text_position = {
        (settings.game.target_size.x - text_size.x) / 2,
        (settings.game.target_size.y - text_size.y) / 2};
    DrawTextEx(*font, text, text_position, 7, 0.0f, WHITE);
}

static void scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene *create_logo_scene()
{
    font = resource_manager.get_pixel7_font();
    logo_scene.init = scene_init;
    logo_scene.update = scene_update;
    logo_scene.render = scene_render;
    logo_scene.cleanup = scene_cleanup;

    return &logo_scene;
};