#include "raylib.h"
#include "main_menu_scene.h"
#include "game.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "game_data.h"

static Scene main_menu_scene;

static Font font1;
static Font font2;

static void scene_init(int arg_count, ...)
{
}

static void scene_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.change(scene_manager.scenes.gameplay, 1, create_game_data("Team1"));
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        exit_game();
    }
}

static void scene_render(void)
{
    DrawTextEx(*resource_manager.get_pixel7_font(), "Block Smashies", (Vector2){0.0f, 0.0f}, 7, 0, WHITE);
}

static void scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene *create_main_menu_scene()
{
    main_menu_scene.init = scene_init;
    main_menu_scene.update = scene_update;
    main_menu_scene.render = scene_render;
    main_menu_scene.cleanup = scene_cleanup;

    return &main_menu_scene;
};