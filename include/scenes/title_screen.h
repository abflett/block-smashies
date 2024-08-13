#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "scene.h"

extern Scene title_screen_scene;

void title_screen_init(void);
void title_screen_update(float delta_time);
void title_screen_render(void);
void title_screen_cleanup(void);

#endif