#include "raylib.h"
#include "embark_scene.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "game_data.h"

Scene embark_scene;

Texture2D *embark_room;

static void scene_init(int arg_count, va_list args)
{
}

static void scene_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.change(scene_manager.scenes.gameplay, 1, create_game_data("Team1"));
    }
}

static void scene_render(void)
{
    DrawTexture(*embark_room, 0, 0, WHITE);
}

static void scene_cleanup(void)
{
}

Scene *create_embark_scene(void)
{
    embark_room = &resource_manager.get_texture("embark-room")->texture;

    embark_scene.init = scene_init;
    embark_scene.update = scene_update;
    embark_scene.render = scene_render;
    embark_scene.cleanup = scene_cleanup;

    return &embark_scene;
}
