#ifndef PLAY_STATE_MANAGER_H
#define PLAY_STATE_MANAGER_H

#include "play_state.h"

typedef struct PlayStateManager
{
    PlayState *current_state;
    PlayState *next_state;

    void (*change_state)(PlayState *new_state);
    void (*update_state)(float delta_time);
    void (*render_state)(void);
} PlayStateManager;

extern PlayStateManager play_state_manager;

#endif