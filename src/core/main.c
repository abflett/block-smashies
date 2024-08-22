#include "raylib.h"
#include "time.h"
#include "game_settings.h"
#include "scene_manager.h"
#include "resource_manager.h"

#define CLEAR_COLOR \
    (Color) { 46, 34, 47, 255 }

RenderTexture2D target_texture;

static void init_game(void)
{
    init_game_from_settings("settings.json");
    target_texture = LoadRenderTexture(game_settings.target_width, game_settings.target_height);
    resource_manager.load_resource_file("assets/config/resources.json");
    scene_manager.change(scene_manager.scenes.logo);
}

static void update_game(float delta_time)
{
    scene_manager.update(delta_time);

    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_C) || WindowShouldClose())
    {
        resource_manager.cleanup();
        game_settings.exitWindow = true;
    }
}

static void draw_game(void)
{
    // Draw the current scene to the render texture
    BeginTextureMode(target_texture);
    ClearBackground(CLEAR_COLOR);
    scene_manager.render();
    EndTextureMode();

    // Draw the render texture to the screen with scaling
    BeginDrawing();
    {
        DrawTexturePro(target_texture.texture,
                       (Rectangle){0, 0, (float)game_settings.target_width, -(float)game_settings.target_height},
                       (Rectangle){0, 0, (float)game_settings.screen_width, (float)game_settings.screen_height},
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);
    }
    EndDrawing();
}

static void close_game(void)
{
    if (scene_manager.current_scene && scene_manager.current_scene->cleanup)
    {
        scene_manager.current_scene->cleanup();
    }

    UnloadRenderTexture(target_texture);
    CloseWindow();
}

int main(void)
{
    srand((unsigned int)time(NULL));
    init_game();

    while (!game_settings.exitWindow)
    {
        float delta_time = GetFrameTime();
        update_game(delta_time);
        draw_game();
    }

    close_game();
    return 0;
}