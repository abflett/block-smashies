#include <time.h>
#include <stdbool.h>
#include "game.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "scene_manager.h"
#include "game_state_manager.h"

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
    ClearBackground(settings.game.clear_color);
    scene_manager.render();
    EndTextureMode();

    // Draw the render texture to the screen with scaling
    BeginDrawing();
    {
        DrawTexturePro(target_texture.texture,
                       (Rectangle){0, 0, settings.game.target_size.x, -settings.game.target_size.y},
                       (Rectangle){0, 0, settings.config.screen_size.x, settings.config.screen_size.y},
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

    initialize_settings();
    srand((unsigned int)time(NULL));
    InitWindow((int)settings.config.screen_size.x, (int)settings.config.screen_size.y, game_title);
    SetExitKey(KEY_NULL); // Disable default exit key (ESC)
    SetTargetFPS(60);     // Set target FPS for the game loop
    initialize_scene_manager();
    initialize_game_state_manager();

    // Todo: Allow other available full screen resolutions other then native
    if (settings.config.fullscreen)
    {
        int monitor = GetCurrentMonitor();
        settings.config.screen_size.x = (float)GetMonitorWidth(monitor);
        settings.config.screen_size.y = (float)GetMonitorHeight(monitor);

        SetWindowSize((int)settings.config.screen_size.x, (int)settings.config.screen_size.y);
        ToggleFullscreen();
    }

    target_texture = LoadRenderTexture((int)settings.game.target_size.x, (int)settings.game.target_size.y);
    resource_manager.load_resource_file(resource_filename);
    scene_manager.change(scene_manager.scenes.logo);

    return game;
}
