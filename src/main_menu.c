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
    DrawText("Main Menu", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Gameplay", 5, 16, 8, WHITE);
}

void main_menu_cleanup(void)
{
    // Cleanup resources used by the main menu
}