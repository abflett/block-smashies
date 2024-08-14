#include "raylib.h"
#include "main.h"
#include "game_settings.h"
#include "scene_manager.h"
#include "logo_scene.h"
#include "gameplay_scene.h"
#include "playing_state.h"
#include "resource_manager.h"

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
    SetTraceLogLevel(LOG_ALL);
    init_game_from_settings("settings.json");
    target_texture = LoadRenderTexture(game_settings.target_width, game_settings.target_height);
    resource_manager.load_resource_file("assets/config/resources.json");
    scene_manager.change(&logo_scene);
}

void update_game(float delta_time)
{
    scene_manager.update(delta_time);

    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_C) || WindowShouldClose())
    {
        // Ensure cleanup is done before exiting
        if (playing_state.cleanup)
        {
            game_settings.is_paused = false; // Reset the pause flag
            playing_state.cleanup();         // Call cleanup
            resource_manager.cleanup();
        }
        game_settings.exitWindow = true;
    }
}

void draw_game(void)
{
    // Draw the current scene to the render texture
    BeginTextureMode(target_texture);
    ClearBackground(BLACK);
    scene_manager.render();
    EndTextureMode();

    // Draw the render texture to the screen with scaling
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