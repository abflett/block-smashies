#ifndef MAIN_H
#define MAIN_H

#include "scene_manager.h"

void init_game(void);
void update_game(float delta_time);
void draw_game(void);
void close_game(void);

extern SceneManager scene_manager;

#endif