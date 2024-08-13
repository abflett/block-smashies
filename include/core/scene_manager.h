#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "scene.h"

typedef struct SceneManager
{
    Scene *current_scene;
    Scene *next_scene;

    void (*change_scene)(Scene *new_scene);
    void (*update_scene)(float delta_time);
    void (*render_scene)(void);
} SceneManager;

extern SceneManager scene_manager;

#endif