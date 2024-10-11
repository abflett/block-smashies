#include "raylib.h"
#include "title_scene.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "settings.h"

static Scene title_scene;
static float min_scene_time = 0.0f;
static Font *font;

static void scene_init(int arg_count, va_list args)
{
    min_scene_time = 0.2f;
}

static void scene_update(float delta_time)
{
    if (min_scene_time >= 0.0f)
    {
        min_scene_time -= delta_time;
    }

    if ((GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP) || GetGamepadButtonPressed() != 0) && min_scene_time <= 0.0f)
    {
        scene_manager.change(scene_manager.scenes.main_menu, 0);
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
}

Scene *create_title_scene()
{
    font = resource_manager.get_pixel7_font();
    title_scene.init = scene_init;
    title_scene.update = scene_update;
    title_scene.render = scene_render;
    title_scene.cleanup = scene_cleanup;

    return &title_scene;
};