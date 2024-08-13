#include "raylib.h"
#include "title_screen.h"
#include "scene_manager.h"
#include "main_menu.h"

Scene title_screen_scene = {
    .init = title_screen_init,
    .update = title_screen_update,
    .render = title_screen_render,
    .cleanup = title_screen_cleanup,
};

void title_screen_init(void)
{
}

void title_screen_update(float delta_time)
{
    if (GetKeyPressed() != 0 || IsGestureDetected(GESTURE_TAP))
    {
        scene_manager.next_scene = &main_menu_scene;
    }
}

void title_screen_render(void)
{
    DrawText("Title Screen - (Animation for the Title Screen)", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Main Menu", 5, 16, 8, WHITE);
}

void title_screen_cleanup(void)
{
}