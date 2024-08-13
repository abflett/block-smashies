#ifndef GAMEPLAY_SCENE_H
#define GAMEPLAY_SCENE_H

#include "scene.h"
#include "game_state_manager.h"

extern Scene gameplay_scene;
extern GameStateManager game_state_manager;

void gameplay_scene_init(void);
void gameplay_scene_update(float delta_time);
void gameplay_scene_render(void);
void gameplay_scene_cleanup(void);

#endif