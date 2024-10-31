#include "raylib.h"
#include "pause_menu_state.h"
#include "game_state_manager.h"
#include "scene_manager.h"
#include "settings.h"

static GameState pause_menu_state;

static void state_init(const int arg_count, va_list args)
{
}

static void state_update(const float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        if (game_state_manager.states.playing->cleanup)
        {
            game_state_manager.states.playing->cleanup();
        }
        scene_manager.change(scene_manager.scenes.logo, 0);
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_state_manager.change(game_state_manager.states.playing, 0, NULL);
    }
}

static void state_render(void)
{
    game_state_manager.context->render();

    DrawRectangle(0, (int)(settings.game.target_size.y / 2) - 40, (int)settings.game.target_size.x, 80, (Color){0, 0, 0, 170});

    // Todo: fix this to use the game font and proper text measure.
    const char *text = "'Escape' to exit\n'Enter' to continue playing!";
    const int fontSize = 8;
    const int textWidth = MeasureText(text, fontSize);
    const int posX = (int)(settings.game.target_size.x - (float)textWidth / 2);
    const int posY = (int)(settings.game.target_size.y / 2 - (float)fontSize / 2 - (float)fontSize);

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