#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "scene.h"

extern Scene title_scene;

void title_scene_init(void);
void title_scene_update(float delta_time);
void title_scene_render(void);
void title_scene_cleanup(void);

#endif