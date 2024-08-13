#include "raylib.h"
#include "main_menu.h"
#include "game_settings.h"
#include "scene_manager.h"
#include "gameplay.h"

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

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.exitWindow = true;
    }
}

void main_menu_render(void)
{
    DrawText("New Game", 5, 16, 8, WHITE);
    DrawText("Continue", 5, 32, 8, DARKGRAY);
    DrawText("Multiplayer ", 5, 48, 8, LIGHTGRAY);
    DrawText("Options", 5, 64, 8, LIGHTGRAY);
    DrawText("Credits", 5, 80, 8, LIGHTGRAY);
    DrawText("Exit", 5, 96, 8, LIGHTGRAY);
}

void main_menu_cleanup(void)
{
    // Cleanup resources used by the main menu
}