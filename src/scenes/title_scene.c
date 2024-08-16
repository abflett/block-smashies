#include "raylib.h"
#include "title_scene.h"
#include "scene_manager.h"
#include "main_menu_scene.h"

void title_scene_init(void)
{
}

void title_scene_update(float delta_time)
{
    if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP))
    {
        scene_manager.next_scene = &main_menu_scene;
    }
}

void title_scene_render(void)
{
    DrawText("Title Screen - (Animation for the Title Screen)", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Main Menu", 5, 16, 8, WHITE);
}

void title_scene_cleanup(void)
{
}

Scene title_scene = {
    .init = title_scene_init,
    .update = title_scene_update,
    .render = title_scene_render,
    .cleanup = title_scene_cleanup,
};