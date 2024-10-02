#include "raylib.h"
#include "embark_scene.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "game_data.h"

Scene embark_scene;

Texture2D *floor_dock;
Texture2D *floor_simulation;
Texture2D *holo_beam;
Texture2D *holo_spots;
Texture2D *num_1;
Texture2D *num_2;
Texture2D *num_3;
Texture2D *num_4;
Texture2D *embark_projector_beams;
Texture2D *embark_room;
Texture2D *embark_text_backing;

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

    DrawTexture(*floor_dock, 22, 166, WHITE);
    DrawTexture(*floor_simulation, 214, 166, WHITE);

    DrawTexture(*embark_projector_beams, 51, 36, WHITE);
    DrawTexture(*holo_spots, 48, 13, WHITE);

    DrawTexture(*holo_beam, 114, 42, WHITE);
    DrawTexture(*embark_text_backing, 114, 51, WHITE);

    DrawTexture(*num_1, 133, 148, WHITE);
    DrawTexture(*num_2, 185, 148, WHITE);
    DrawTexture(*num_3, 81, 148, WHITE);
    DrawTexture(*num_4, 236, 148, WHITE);
}

static void scene_cleanup(void)
{
}

Scene *create_embark_scene(void)
{
    floor_dock = &resource_manager.get_texture("embark-floor-dock")->texture;
    floor_simulation = &resource_manager.get_texture("embark-floor-simulation")->texture;
    holo_beam = &resource_manager.get_texture("embark-holo-beam")->texture;
    holo_spots = &resource_manager.get_texture("embark-holo-spots")->texture;
    num_1 = &resource_manager.get_texture("embark-num-1")->texture;
    num_2 = &resource_manager.get_texture("embark-num-2")->texture;
    num_3 = &resource_manager.get_texture("embark-num-3")->texture;
    num_4 = &resource_manager.get_texture("embark-num-4")->texture;
    embark_projector_beams = &resource_manager.get_texture("embark-projector-beams")->texture;
    embark_room = &resource_manager.get_texture("embark-room")->texture;
    embark_text_backing = &resource_manager.get_texture("embark-text-backing")->texture;

    embark_scene.init = scene_init;
    embark_scene.update = scene_update;
    embark_scene.render = scene_render;
    embark_scene.cleanup = scene_cleanup;

    return &embark_scene;
}
