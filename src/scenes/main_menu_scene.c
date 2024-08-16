#include "raylib.h"
#include "main_menu_scene.h"
#include "game_settings.h"
#include "scene_manager.h"

static void scene_init(void)
{
    // Initialize the main menu scene
}

static void scene_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.next_scene = scene_manager.scenes.gameplay;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.exitWindow = true;
    }
}

static void scene_render(void)
{
    DrawText("New Game", 5, 16, 8, WHITE);
    DrawText("Continue", 5, 32, 8, DARKGRAY);
    DrawText("Multiplayer ", 5, 48, 8, LIGHTGRAY);
    DrawText("Options", 5, 64, 8, LIGHTGRAY);
    DrawText("Credits", 5, 80, 8, LIGHTGRAY);
    DrawText("Exit", 5, 96, 8, LIGHTGRAY);
}

static void scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene main_menu_scene = {
    .init = scene_init,
    .update = scene_update,
    .render = scene_render,
    .cleanup = scene_cleanup,
};