#include <stddef.h>

#include "game_state_manager.h"
#include "game_state.h"
#include "playing_state.h"
#include "pause_menu_state.h"
#include "game_over_state.h"
#include "game_intro_start_state.h"
#include "high_score_state.h"
#include "upgrade_menu_state.h"
#include "settings_menu_state.h"

void change_state(GameState *new_state, const int arg_count, ...)
{
    if (game_state_manager.current_state && game_state_manager.current_state->cleanup)
    {
        game_state_manager.current_state->cleanup();
    }

    game_state_manager.current_state = new_state;

    if (game_state_manager.current_state && game_state_manager.current_state->init)
    {
        va_list args;
        va_start(args, arg_count);
        game_state_manager.current_state->init(arg_count, args);
        va_end(args);
    }
}

void update_state(const float delta_time)
{
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
    .states.playing = NULL,
    .states.pause_menu = NULL,
    .states.game_over = NULL,
    .states.game_intro_start = NULL,
    .change = change_state,
    .update = update_state,
    .render = render_state};

void initialize_game_state_manager(void)
{
    game_state_manager.states.playing = create_playing_state();
    game_state_manager.states.pause_menu = create_pause_menu_state();
    game_state_manager.states.game_over = create_game_over_state();
    game_state_manager.states.game_intro_start = create_game_intro_start_state();
    game_state_manager.states.high_score = create_high_score_state();
    game_state_manager.states.upgrade_menu = create_upgrade_menu_state();
    game_state_manager.states.settings_menu = create_settings_menu_state();
}