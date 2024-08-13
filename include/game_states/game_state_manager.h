#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "game_state.h"

typedef struct GameStateManager
{
    GameState *current_state;
    GameState *next_state;

    void (*change)(GameState *new_state);
    void (*update)(float delta_time);
    void (*render)(void);
} GameStateManager;

extern GameStateManager game_state_manager;

#endif