#include "raylib.h"
#include "main.h"
#include "scene_manager.h"
#include "logo_screen.h"
#include "gameplay.h"

SceneManager scene_manager;

bool exitWindow = false;
bool exitWindowRequested = false;
int screen_width = 1280;
int screen_height = 720;
bool fullscreen = false;

RenderTexture2D target_texture; // Render texture target
int target_width = 320;
int target_height = 180;

int main(void)
{
    init_game();

    while (!exitWindow)
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
    set_game_resolution(&screen_width, &screen_height, fullscreen);
    SetExitKey(KEY_NULL); // Disable default exit key (ESC)
    SetTargetFPS(60);     // Set target FPS for the game loop

    target_texture = LoadRenderTexture(target_width, target_height);

    scene_manager_change_scene(&scene_manager, &gameplay_scene);
}

void update_game(float delta_time)
{
    scene_manager_update(&scene_manager, delta_time);

    if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
        exitWindowRequested = true;

    // Handle exit confirmation
    if (exitWindowRequested)
    {
        if (IsKeyPressed(KEY_Y))
            exitWindow = true;
        else if (IsKeyPressed(KEY_N))
            exitWindowRequested = false;
    }
}

void draw_game(void)
{

    BeginTextureMode(target_texture);
    ClearBackground(BLACK);

    if (exitWindowRequested)
    {
        // Display exit confirmation dialog
        DrawRectangle(0, (target_height / 2) - 25, target_width, 50, DARKPURPLE);
        DrawText("Are you sure you want to exit the game? [Y/N]", 5, (target_height / 2) - 5, 8, WHITE);
    }
    else
    {
        // Render the current scene
        scene_manager_render(&scene_manager);
    }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    // Draw the target texture scaled to fit the screen
    DrawTexturePro(target_texture.texture,
                   (Rectangle){0, 0, (float)target_width, -(float)target_height},
                   (Rectangle){0, 0, (float)screen_width, (float)screen_height},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);

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