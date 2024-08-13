#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "scene.h"

extern Scene main_menu_scene;

void main_menu_init(void);
void main_menu_update(float delta_time);
void main_menu_render(void);
void main_menu_cleanup(void);

#endif