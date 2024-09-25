#include <stddef.h>
#include "scene_manager.h"
#include "scene.h"

// scenes
#include "gameplay_scene.h"
#include "title_scene.h"
#include "main_menu_scene.h"
#include "logo_scene.h"

void change_scene(Scene *new_scene, int arg_count, ...)
{
    if (scene_manager.current_scene && scene_manager.current_scene->cleanup)
        scene_manager.current_scene->cleanup();

    scene_manager.current_scene = new_scene;
    if (scene_manager.current_scene && scene_manager.current_scene->init)
    {
        va_list args;
        va_start(args, arg_count);

        // Call the init function with arg_count and variable arguments
        scene_manager.current_scene->init(arg_count, args);

        va_end(args);
    }
}

void update_scene(float delta_time)
{
    if (scene_manager.current_scene && scene_manager.current_scene->update)
        scene_manager.current_scene->update(delta_time);
}

void render_scene(void)
{
    if (scene_manager.current_scene && scene_manager.current_scene->render)
        scene_manager.current_scene->render();
}

SceneManager scene_manager = {
    .current_scene = NULL,
    .scenes.logo = NULL,
    .scenes.title = NULL,
    .scenes.main_menu = NULL,
    .scenes.gameplay = NULL,
    .change = change_scene,
    .update = update_scene,
    .render = render_scene};

void initialize_scene_manager(void)
{
    scene_manager.scenes.logo = create_logo_scene();
    scene_manager.scenes.title = create_title_scene();
    scene_manager.scenes.main_menu = create_main_menu_scene();
    scene_manager.scenes.gameplay = create_gameplay_scene();
}