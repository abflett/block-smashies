#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include <stdarg.h>
#include "game_state.h"

typedef struct GameStates
{
    GameState *playing;
    GameState *pause_menu;
    GameState *game_over;
} GameStates;

typedef struct GameStateManager
{
    GameState *current_state;
    GameStates states;

    void (*change)(GameState *new_state, int argc, ...);
    void (*update)(float delta_time);
    void (*render)(void);
} GameStateManager;

extern GameStateManager game_state_manager;

#endif