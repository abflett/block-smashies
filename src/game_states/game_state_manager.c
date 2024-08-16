#include <stddef.h>
#include "raylib.h"
#include "game_state_manager.h"
#include "game_state.h"

// GameStates
#include "playing_state.h"
#include "pause_menu_state.h"
#include "game_over_state.h"

void change_state(GameState *new_state)
{
    if (game_state_manager.current_state && game_state_manager.current_state->cleanup)
    {
        game_state_manager.current_state->cleanup();
    }

    game_state_manager.current_state = new_state;

    if (game_state_manager.current_state && game_state_manager.current_state->init)
    {
        game_state_manager.current_state->init();
    }
}

void update_state(float delta_time)
{
    if (game_state_manager.next_state)
    {
        change_state(game_state_manager.next_state);
        game_state_manager.next_state = NULL; // Clear the next_state pointer
    }

    if (game_state_manager.current_state && game_state_manager.current_state->update)
    {
        game_state_manager.current_state->update(delta_time);
    }
}

void render_state(void)
{
    if (game_state_manager.current_state && game_state_manager.current_state->render)
    {
        game_state_manager.current_state->render();
    }
}

GameStateManager game_state_manager = {
    .current_state = NULL,
    .next_state = NULL,
    .states.playing = &playing_state,
    .states.pause_menu = &pause_menu_state,
    .states.game_over = &game_over_state,
    .change = change_state,
    .update = update_state,
    .render = render_state};