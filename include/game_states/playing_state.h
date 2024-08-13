#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "game_state.h"

extern GameState playing_state;

void playing_state_init(void);
void playing_state_update(float delta_time);
void playing_state_render(void);
void playing_state_cleanup(void);

#endif