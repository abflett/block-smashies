#include <time.h>
#include <stdbool.h>
#include "game.h"
#include "game_settings.h"
#include "scene_manager.h"
#include "resource_manager.h"

#define CLEAR_COLOR \
    (Color) { 46, 34, 47, 255 }

static RenderTexture2D target_texture; // Now private to game.c
static bool exit_window = false;       // Now private to game.c
static Game *game = NULL;              // Private pointer to the game instance

static void update_game(float delta_time)
{
    scene_manager.update(delta_time);

    if (((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_C)) || WindowShouldClose())
    {

        exit_window = true;
    }
}

static void render_game(void)
{
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

static void cleanup_game(void)
{
    if (scene_manager.current_scene && scene_manager.current_scene->cleanup)
    {
        scene_manager.current_scene->cleanup();
    }

    resource_manager.cleanup();
    UnloadRenderTexture(target_texture);
    CloseWindow();
    free(game);
}

static void run_game(void)
{
    while (!exit_window)
    {
        float delta_time = GetFrameTime();
        update_game(delta_time);
        render_game();
    }

    cleanup_game();
}

void exit_game(void)
{
    exit_window = true;
}

Game *create_game(const char *game_title, const char *settings_filename, const char *resource_filename)
{
    game = malloc(sizeof(Game));
    exit_window = false;
    game->run = run_game;

    initialize_game_settings(settings_filename);
    srand((unsigned int)time(NULL));
    InitWindow(game_settings.screen_width, game_settings.screen_height, game_title);
    SetExitKey(KEY_NULL); // Disable default exit key (ESC)
    SetTargetFPS(60);     // Set target FPS for the game loop

    // Todo: Allow other available full screen resolutions other then native
    if (game_settings.fullscreen)
    {
        int monitor = GetCurrentMonitor();
        game_settings.screen_width = GetMonitorWidth(monitor);
        game_settings.screen_height = GetMonitorHeight(monitor);

        SetWindowSize(game_settings.screen_width, game_settings.screen_height);
        ToggleFullscreen();
    }

    target_texture = LoadRenderTexture(game_settings.target_width, game_settings.target_height);
    resource_manager.load_resource_file(resource_filename);
    scene_manager.change(scene_manager.scenes.logo);

    return game;
}
