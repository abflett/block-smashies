#ifndef PAUSE_MENU_STATE_H
#define PAUSE_MENU_STATE_H

#include "game_state.h"

extern GameState pause_menu_state;

void pause_menu_state_init(void);
void pause_menu_state_update(float delta_time);
void pause_menu_state_render(void);
void pause_menu_state_cleanup(void);

#endif