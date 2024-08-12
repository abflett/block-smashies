#include "scene_manager.h"
#include "scene.h"
#include <stddef.h>

void scene_manager_change_scene(SceneManager *manager, Scene *new_scene)
{
    if (manager->current_scene && manager->current_scene->cleanup)
    {
        manager->current_scene->cleanup();
    }

    manager->current_scene = new_scene;
    if (manager->current_scene && manager->current_scene->init)
    {
        manager->current_scene->init();
    }
}

void scene_manager_update(SceneManager *manager, float delta_time)
{
    if (manager->next_scene)
    {
        scene_manager_change_scene(manager, manager->next_scene);
        manager->next_scene = NULL; // Clear the next_scene pointer
    }

    if (manager->current_scene && manager->current_scene->update)
    {
        manager->current_scene->update(delta_time);
    }
}

void scene_manager_render(SceneManager *manager)
{
    if (manager->current_scene && manager->current_scene->render)
    {
        manager->current_scene->render();
    }
}