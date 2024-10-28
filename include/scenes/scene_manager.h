#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <stdarg.h>
#include "scene.h"

typedef struct GameScenes
{
    Scene *gameplay;
    Scene *logo;
    Scene *main_menu;
    Scene *title;
    Scene *embark;
} GameScenes;

typedef struct SceneManager
{
    Scene *current_scene;
    GameScenes scenes;

    void (*change)(Scene *new_scene, const int arg_count, ...);
    void (*update)(const float delta_time);
    void (*render)(void);
} SceneManager;

extern SceneManager scene_manager;
void initialize_scene_manager(void);

#endif
