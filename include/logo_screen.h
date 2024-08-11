#ifndef LOGO_SCREEN_H
#define LOGO_SCREEN_H

#include "scene.h"

extern Scene logo_screen_scene;

void logo_screen_init(void);
void logo_screen_update(float delta_time);
void logo_screen_render(void);
void logo_screen_cleanup(void);

#endif