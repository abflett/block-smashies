#include "raylib.h"
#include "main_menu_scene.h"
#include "game_settings.h"
#include "scene_manager.h"
#include "gameplay_scene.h"

void main_menu_scene_init(void)
{
    // Initialize the main menu scene
}

void main_menu_scene_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.next_scene = &gameplay_scene;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.exitWindow = true;
    }
}

void main_menu_scene_render(void)
{
    DrawText("New Game", 5, 16, 8, WHITE);
    DrawText("Continue", 5, 32, 8, DARKGRAY);
    DrawText("Multiplayer ", 5, 48, 8, LIGHTGRAY);
    DrawText("Options", 5, 64, 8, LIGHTGRAY);
    DrawText("Credits", 5, 80, 8, LIGHTGRAY);
    DrawText("Exit", 5, 96, 8, LIGHTGRAY);
}

void main_menu_scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene main_menu_scene = {
    .init = main_menu_scene_init,
    .update = main_menu_scene_update,
    .render = main_menu_scene_render,
    .cleanup = main_menu_scene_cleanup,
};