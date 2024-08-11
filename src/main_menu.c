#include "main_menu.h"
#include "raylib.h"
#include "scene_manager.h"
#include "gameplay.h"

extern SceneManager scene_manager;

Scene main_menu_scene = {
    .init = main_menu_init,
    .update = main_menu_update,
    .render = main_menu_render,
    .cleanup = main_menu_cleanup,
};

void main_menu_init(void)
{
    // Initialize the main menu scene
}

void main_menu_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.next_scene = &gameplay_scene;
    }
}

void main_menu_render(void)
{
    DrawText("Main Menu", 190, 200, 20, DARKGRAY);
    DrawText("Press ENTER to Gameplay", 190, 250, 20, DARKGRAY);
}

void main_menu_cleanup(void)
{
    // Cleanup resources used by the main menu
}