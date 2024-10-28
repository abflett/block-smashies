#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include <stdarg.h>
#include "game_state.h"
#include "game_context.h"

typedef struct GameStates
{
    GameState *playing;
    GameState *pause_menu;
    GameState *game_over;
    GameState *game_intro_start;
    GameState *high_score;
    GameState *upgrade_menu;
    GameState *settings_menu;
} GameStates;

typedef struct GameStateManager
{
    GameState *current_state;
    GameStates states;
    GameContext *context;

    void (*change)(GameState *new_state, const int arg_count, ...);
    void (*update)(const float delta_time);
    void (*render)(void);
} GameStateManager;

extern GameStateManager game_state_manager;

void initialize_game_state_manager(void);

#endif
