#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "scene.h"

typedef struct SceneManager
{
    Scene *current_scene;
    Scene *next_scene;
} SceneManager;

void scene_manager_change_scene(SceneManager *manager, Scene *new_scene);
void scene_manager_update(SceneManager *manager, float delta_time);
void scene_manager_render(SceneManager *manager);

#endif