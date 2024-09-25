#include "raylib.h"
#include "title_scene.h"
#include "scene_manager.h"

static Scene title_scene;

static void scene_init(int arg_count, ...)
{
}

static void scene_update(float delta_time)
{
    if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP))
    {
        scene_manager.change(scene_manager.scenes.main_menu, 0);
    }
}

static void scene_render(void)
{
    DrawText("Title Screen - (Animation for the Title Screen)", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Main Menu", 5, 16, 8, WHITE);
}

static void scene_cleanup(void)
{
}

Scene *create_title_scene()
{
    title_scene.init = scene_init;
    title_scene.update = scene_update;
    title_scene.render = scene_render;
    title_scene.cleanup = scene_cleanup;

    return &title_scene;
};