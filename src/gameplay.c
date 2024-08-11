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
    DrawText("Gameplay", 190, 200, 20, DARKGRAY);
    DrawText("Press ENTER to Main Menu", 190, 250, 20, DARKGRAY);
}

void gameplay_cleanup(void)
{
    // Cleanup resources used by the main menu
}