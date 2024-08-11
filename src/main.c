#include "raylib.h"
#include "main.h"
#include "scene_manager.h"
#include "logo_screen.h"

SceneManager scene_manager;

bool exitWindow = false;
bool exitWindowRequested = false;
int screen_width = 1280;
int screen_height = 720;
bool fullscreen = true;

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
    scene_manager_change_scene(&scene_manager, &logo_screen_scene);
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
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (exitWindowRequested)
    {
        // Display exit confirmation dialog
        DrawRectangle(0, (screen_height / 2) - 100, screen_width, 200, DARKGRAY);
        DrawText("Are you sure you want to exit the game? [Y/N]", 40, (screen_height / 2) - 15, 30, WHITE);
    }
    else
    {
        // Render the current scene
        scene_manager_render(&scene_manager);
    }

    EndDrawing();
}

void close_game(void)
{
    if (scene_manager.current_scene && scene_manager.current_scene->cleanup)
    {
        scene_manager.current_scene->cleanup();
    }
    CloseWindow();
}