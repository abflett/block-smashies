#ifndef LOGO_SCENE_H
#define LOGO_SCENE_H

#include "scene.h"

extern Scene logo_scene;

void logo_scene_init(void);
void logo_scene_update(float delta_time);
void logo_scene_render(void);
void logo_scene_cleanup(void);

#endif