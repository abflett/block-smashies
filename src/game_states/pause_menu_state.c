#include "raylib.h"
#include "pause_menu_state.h"
#include "game_state_manager.h"
#include "resource_manager.h"
#include "scene_manager.h"
#include "settings.h"
#include "entities.h"
#include "resource_manager.h"

static GameState pause_menu_state;

static void state_init(void)
{
}

static void state_update(float delta_time)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (game_state_manager.states.playing->cleanup)
        {
            game_state_manager.states.playing->cleanup();
        }
        scene_manager.change(scene_manager.scenes.logo, 0);
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        game_state_manager.change(game_state_manager.states.playing);
    }
}

static void state_render(void)
{
    game_state_manager.context->render();

    DrawRectangle(0, (int)(settings.game.target_size.y / 2) - 40, (int)settings.game.target_size.x, 80, (Color){0, 0, 0, 170});

    const char *text = "'Escape' to exit\n'Enter' to continue playing!";
    int fontSize = 8;
    int textWidth = MeasureText(text, fontSize);
    int posX = (int)((settings.game.target_size.x - textWidth) / 2);
    int posY = (int)((settings.game.target_size.y / 2) - (fontSize / 2)) - fontSize;

    DrawText(text, posX, posY, fontSize, LIGHTGRAY);
}

static void state_cleanup(void)
{
}

GameState *create_pause_menu_state(void)
{
    pause_menu_state.init = state_init;
    pause_menu_state.update = state_update;
    pause_menu_state.render = state_render;
    pause_menu_state.cleanup = state_cleanup;

    return &pause_menu_state;
};