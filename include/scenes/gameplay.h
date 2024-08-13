#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "scene.h"
#include "game_state_manager.h"

extern Scene gameplay_scene;
extern GameStateManager game_state_manager;

void gameplay_init(void);
void gameplay_update(float delta_time);
void gameplay_render(void);
void gameplay_cleanup(void);

#endif