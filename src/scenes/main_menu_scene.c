#include "raylib.h"
#include "main_menu_scene.h"
#include "game.h"
#include "scene_manager.h"

static Scene main_menu_scene;

static Font font1;
static Font font2;

static void scene_init(void)
{
    // Initialize the main menu scene
    font1 = LoadFont("assets/fonts/pixel7.fnt");
    font2 = LoadFontEx("assets/fonts/Tiny5-Regular.ttf", 9, 0, 0); // works and looks nice tiny5fnt.fnt
}

static void scene_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.next_scene = scene_manager.scenes.gameplay;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        exit_game();
    }
}

static void scene_render(void)
{
    // DrawTextEx(font1, "jjjjjjjjjjj\njjjjjjjjj", (Vector2){0.0f, 0.0f}, 8, 0, WHITE);
    //   DrawTextEx(font1, "1: ~!@#$^&*()_+=-`[]{}|;':,./<>?", (Vector2){0.0f, 0.0f}, 5, 0, WHITE);
    DrawTextEx(font1, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890", (Vector2){0.0f, 0.0f}, 7, 0, WHITE);
    DrawTextEx(font2, "2: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijk\nlmnopqrstuvwxyz1234567890-=~!@#$=^&*()_+", (Vector2){0.0f, 110.0f}, 9, 0, WHITE);
    // DrawTextEx(font2, "2: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-=~!@#$=^&*()_+", (Vector2){2.0f, 30.0f}, 18, 0, WHITE);
    // DrawTextEx(font2, "2: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-=~!@#$=^&*()_+", (Vector2){2.0f, 45.0f}, 27, 0, WHITE);
    // DrawTextEx(font2, "2: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-=~!@#$=^&*()_+", (Vector2){2.0f, 60.0f}, 36, 0, WHITE);
    // DrawText("9: ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-=~!@#$=^&*()_+", 2, 100, 8, WHITE);
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