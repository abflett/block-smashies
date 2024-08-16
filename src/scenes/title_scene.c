#include "raylib.h"
#include "title_scene.h"
#include "scene_manager.h"

static void scene_init(void)
{
}

static void scene_update(float delta_time)
{
    if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP))
    {
        scene_manager.next_scene = scene_manager.scenes.main_menu;
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

Scene title_scene = {
    .init = scene_init,
    .update = scene_update,
    .render = scene_render,
    .cleanup = scene_cleanup,
};