#include "raylib.h"
#include "main.h"
#include "game_settings.h"
#include "scene_manager.h"
#include "logo_scene.h"
#include "gameplay_scene.h"

RenderTexture2D target_texture; // Render texture target

int main(void)
{
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

void init_game(void)
{
    init_game_from_settings("settings.json");
    SetExitKey(KEY_NULL); // Disable default exit key (ESC) - not used until exit scene is created
    SetTargetFPS(60);     // Set target FPS for the game loop
    target_texture = LoadRenderTexture(game_settings.target_width, game_settings.target_height);
    scene_manager.change(&logo_scene);
}

void update_game(float delta_time)
{
    scene_manager.update(delta_time);

    if (WindowShouldClose())
        game_settings.exitWindow = true;
}

void draw_game(void)
{

    BeginTextureMode(target_texture);
    ClearBackground(BLACK);
    scene_manager.render();
    EndTextureMode();

    BeginDrawing();
    {
        ClearBackground(BLACK);
        DrawTexturePro(target_texture.texture,
                       (Rectangle){0, 0, (float)game_settings.target_width, -(float)game_settings.target_height},
                       (Rectangle){0, 0, (float)game_settings.screen_width, (float)game_settings.screen_height},
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);
    }
    EndDrawing();
}

void close_game(void)
{
    if (scene_manager.current_scene && scene_manager.current_scene->cleanup)
    {
        scene_manager.current_scene->cleanup();
    }

    UnloadRenderTexture(target_texture);
    CloseWindow();
}