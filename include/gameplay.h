#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "scene.h"

extern Scene gameplay_scene;

void gameplay_init(void);
void gameplay_update(float delta_time);
void gameplay_render(void);
void gameplay_cleanup(void);

#endif