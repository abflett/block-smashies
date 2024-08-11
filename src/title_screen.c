#include "title_screen.h"
#include "raylib.h"
#include "scene_manager.h"
#include "main_menu.h"

extern SceneManager scene_manager;

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
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        scene_manager.next_scene = &main_menu_scene;
    }
}

void title_screen_render(void)
{
    DrawText("Title Screen", 190, 200, 20, DARKGRAY);
    DrawText("Press ENTER to Main Menu", 190, 250, 20, DARKGRAY);
}

void title_screen_cleanup(void)
{
    // Cleanup resources used by the main menu
}