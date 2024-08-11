#include "gameplay.h"
#include "raylib.h"
#include "scene_manager.h"
#include "main_menu.h"

extern SceneManager scene_manager;

Scene gameplay_scene = {
    .init = gameplay_init,
    .update = gameplay_update,
    .render = gameplay_render,
    .cleanup = gameplay_cleanup,
};

void gameplay_init(void)
{
    // Initialize the main menu scene
}

void gameplay_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.next_scene = &main_menu_scene;
    }
}

void gameplay_render(void)
{
    DrawText("Gameplay", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Main Menu", 5, 16, 8, WHITE);
    DrawRectangle(150, 170, 20, 5, RED);
}

void gameplay_cleanup(void)
{
    // Cleanup resources used by the main menu
}