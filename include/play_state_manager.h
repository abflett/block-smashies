#ifndef PLAY_STATE_MANAGER_H
#define PLAY_STATE_H

#include "play_state.h"

typedef struct PlayStateManager
{
    PlayState *current_state;
    PlayState *next_state;
} PlayStateManager;

void play_state_manager_change_state(PlayStateManager *manager, PlayState *new_state);
void play_state_manager_update(PlayStateManager *manager, float delta_time);
void play_state_manager_render(PlayStateManager *manager);

#endif