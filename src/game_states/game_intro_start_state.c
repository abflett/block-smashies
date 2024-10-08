#include <stdio.h>
#include <math.h>
#include "raylib.h"

#include "game_intro_start_state.h"
#include "game_state_manager.h"

static GameState game_intro_start_state;

static float count_down;

static void state_init(void)
{
    count_down = 2.0f;
}

static void state_update(float delta_time)
{
    count_down -= delta_time;

    if (count_down < 0)
    {
        game_state_manager.change(game_state_manager.states.playing);
    }

    game_state_manager.context->game_ui->update(delta_time);
}

static void state_render(void)
{
    game_state_manager.context->render();

    // Convert countdown value to text
    char count_down_text[2]; // Increase size if necessary

    // Round up the floating-point number
    int rounded_count_down = (int)ceil(count_down);

    // Format the rounded integer value to a string
    snprintf(count_down_text, sizeof(count_down_text), "%d", rounded_count_down);

    // Draw countdown text
    DrawText(count_down_text, 198, 110, 22, LIGHTGRAY);
}

static void state_cleanup(void)
{
    // Cleanup logic for the intro state
}

// Return a pointer to the statically defined GameState
GameState *create_game_intro_start_state(void)
{
    game_intro_start_state.init = state_init;
    game_intro_start_state.update = state_update;
    game_intro_start_state.render = state_render;
    game_intro_start_state.cleanup = state_cleanup;

    return &game_intro_start_state;
}