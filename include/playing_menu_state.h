#ifndef PLAYING_MENU_STATE_H
#define PLAYING_MENU_STATE_H

#include "play_state.h"

extern PlayState playing_menu_state;

void playing_menu_state_init(void);
void playing_menu_state_update(float delta_time);
void playing_menu_state_render(void);
void playing_menu_state_cleanup(void);

#endif