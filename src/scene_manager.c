#include "scene_manager.h"
#include "scene.h"
#include <stddef.h>

void change_scene_global(Scene *new_scene)
{
    if (scene_manager.current_scene && scene_manager.current_scene->cleanup)
        scene_manager.current_scene->cleanup();

    scene_manager.current_scene = new_scene;
    if (scene_manager.current_scene && scene_manager.current_scene->init)
        scene_manager.current_scene->init();
}

void update_scene_global(float delta_time)
{
    if (scene_manager.next_scene)
    {
        change_scene_global(scene_manager.next_scene);
        scene_manager.next_scene = NULL;
    }

    if (scene_manager.current_scene && scene_manager.current_scene->update)
        scene_manager.current_scene->update(delta_time);
}

void render_scene_global(void)
{
    if (scene_manager.current_scene && scene_manager.current_scene->render)
        scene_manager.current_scene->render();
}

SceneManager scene_manager = {
    .current_scene = NULL,
    .next_scene = NULL,
    .change_scene = change_scene_global,
    .update_scene = update_scene_global,
    .render_scene = render_scene_global};