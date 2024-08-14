#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include "game_state.h"

extern GameState game_over_state;

void game_over_state_init(void);
void game_over_state_update(float delta_time);
void game_over_state_render(void);
void game_over_state_cleanup(void);

#endif