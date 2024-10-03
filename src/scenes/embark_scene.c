#include <stdbool.h>
#include "raylib.h"
#include "embark_scene.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "game_data.h"
#include "ship.h"

#define SHIP_COLORS 25

Scene embark_scene;

Ship *ship_p1;
Ship *ship_p2;
Ship *ship_p3;
Ship *ship_p4;

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

int player_count = 2;

bool enter_exit = true;

int color_selection;

static void change_player_count(int *players)
{
    player_count = *players;

    ship_p1->player_count = player_count;
    ship_p1->segments = ship_p1->calculate_segments(ship_p1);
    ship_p1->ship_shield->set_shield(ship_p1->ship_shield);

    ship_p2->player_count = player_count;
    ship_p2->segments = ship_p2->calculate_segments(ship_p2);
    ship_p2->ship_shield->set_shield(ship_p2->ship_shield);

    ship_p3->player_count = player_count;
    ship_p3->segments = ship_p3->calculate_segments(ship_p3);
    ship_p3->ship_shield->set_shield(ship_p3->ship_shield);

    ship_p4->player_count = player_count;
    ship_p4->segments = ship_p4->calculate_segments(ship_p4);
    ship_p4->ship_shield->set_shield(ship_p4->ship_shield);
}

static void scene_init(int arg_count, va_list args)
{
    ship_p1 = create_ship(1, player_count, GetRandomValue(0, 25), (b2Vec2){135, 80});
    ship_p2 = create_ship(2, player_count, GetRandomValue(0, 25), (b2Vec2){188, 80});
    ship_p3 = create_ship(3, player_count, GetRandomValue(0, 25), (b2Vec2){84, 80});
    ship_p4 = create_ship(4, player_count, GetRandomValue(0, 25), (b2Vec2){239, 80});
}

static void scene_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        if (enter_exit)
        {
            scene_manager.change(scene_manager.scenes.gameplay, 1, create_game_data("Team1"));
        }
        else
        {
            scene_manager.change(scene_manager.scenes.main_menu, 0);
        }
    }

    if (IsKeyPressed(KEY_W))
    {
        color_selection = (1 + color_selection) % SHIP_COLORS;
        ship_p1->ship_body->set_color(ship_p1->ship_body, color_selection);
    }
    if (IsKeyPressed(KEY_S))
    {
        color_selection = (color_selection - 1 + SHIP_COLORS) % SHIP_COLORS;
        ship_p1->ship_body->set_color(ship_p1->ship_body, color_selection);
    }
    if (IsKeyPressed(KEY_A))
    {
        ship_p1->shield_level = (ship_p1->shield_level + 1) % 4;
        ship_p1->ship_shield->set_shield(ship_p1->ship_shield);
    }
    if (IsKeyPressed(KEY_D))
    {
        ship_p1->segments = (ship_p1->segments % 4) + 1;
        ship_p1->ship_shield->set_shield(ship_p1->ship_shield);
    }

    if (IsKeyPressed(KEY_E))
    {
        player_count = (player_count % 4) + 1;
        change_player_count(&player_count);
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        enter_exit = true;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        enter_exit = false;
    }
}

static void scene_render(void)
{
    DrawTexture(*embark_room, 0, 0, WHITE);

    if (enter_exit)
    {
        DrawTexture(*floor_simulation, 214, 166, WHITE);
    }
    else
    {
        DrawTexture(*floor_dock, 22, 166, WHITE);
    }

    DrawTexture(*embark_projector_beams, 51, 36, WHITE);
    DrawTexture(*holo_spots, 48, 13, WHITE);

    DrawTexture(*holo_beam, 114, 42, WHITE);
    DrawTexture(*embark_text_backing, 114, 51, WHITE);

    ship_p1->render(ship_p1);
    DrawTexture(*num_1, 133, 148, WHITE);
    if (player_count >= 2)
    {
        ship_p2->render(ship_p2);
        DrawTexture(*num_2, 185, 148, WHITE);
    }
    if (player_count >= 3)
    {
        ship_p3->render(ship_p3);
        DrawTexture(*num_3, 81, 148, WHITE);
    }
    if (player_count >= 4)
    {
        ship_p4->render(ship_p4);
        DrawTexture(*num_4, 236, 148, WHITE);
    }
}

static void scene_cleanup(void)
{
    ship_p1->cleanup(ship_p1);
    ship_p2->cleanup(ship_p2);
    ship_p3->cleanup(ship_p3);
    ship_p4->cleanup(ship_p4);
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

    color_selection = 0;

    embark_scene.init = scene_init;
    embark_scene.update = scene_update;
    embark_scene.render = scene_render;
    embark_scene.cleanup = scene_cleanup;

    return &embark_scene;
}
