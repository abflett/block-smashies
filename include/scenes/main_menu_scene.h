#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "scene.h"

extern Scene main_menu_scene;

void main_menu_scene_init(void);
void main_menu_scene_update(float delta_time);
void main_menu_scene_render(void);
void main_menu_scene_cleanup(void);

#endif