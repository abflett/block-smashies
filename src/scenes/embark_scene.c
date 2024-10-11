#include <stdbool.h>
#include "raylib.h"
#include "embark_scene.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "game_data.h"
#include "ship.h"
#include "game.h"

#define SHIP_COLORS 25
#define MAX_PLAYERS 4

static Scene embark_scene;
static InputManager *input_manager;
static GameData *game_data;
static Ship *ships[MAX_PLAYERS];

static Texture2D *floor_dock;
static Texture2D *floor_simulation;
static Texture2D *holo_beam;
static Texture2D *holo_spots;
static Texture2D *nums[MAX_PLAYERS];
static Texture2D *embark_projector_beams;
static Texture2D *embark_room;
static Texture2D *embark_text_backing;

static int menu_selection = 1;
static const int menu_selection_count = 2;
static int color_selection = 0;
static bool menu_selection_mode = true;

static void change_player_count(int *players)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        ships[i]->player_count = game_data->player_count;            // should be a pointer reference
        ships[i]->segments = ships[i]->calculate_segments(ships[i]); // maybe check and update inside ship instead
        ships[i]->ship_shield->set_shield(ships[i]->ship_shield);    // maybe check and update inside ship shield instead
    }
}

static void scene_init(int arg_count, va_list args)
{
    menu_selection = 1;
    float x_positions[] = {135, 188, 84, 239};

    SetRandomSeed(1);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        ships[i] = create_ship(i + 1, game_data->player_count, GetRandomValue(0, 24), (b2Vec2){x_positions[i], 80}); // player count and color should be a pointer reference
    }
}

static void scene_update(float delta_time)
{
    InputMapping *player_inputs[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        player_inputs[i] = input_manager->get_player_input(i);
    }

    if (menu_selection_mode)
    {

        if (input_manager->check_for_new_players(game_data->player_count))
        {
            game_data->player_count++;
            change_player_count(&game_data->player_count); // update the ships
        }

        if (IsKeyPressed(player_inputs[0]->action_k_ENTER) || IsGamepadButtonPressed(0, player_inputs[0]->action_A) || IsGamepadButtonPressed(0, player_inputs[0]->action_START))
        {
            switch (menu_selection)
            {
            case 0:
                scene_manager.change(scene_manager.scenes.main_menu, 0);
                break;
            case 1:
                scene_manager.change(scene_manager.scenes.gameplay, 1, game_data);
                break;
            default:
                scene_manager.change(scene_manager.scenes.main_menu, 0);
                break;
            }
        }

        if (IsKeyPressed(player_inputs[0]->action_k_RIGHT) || IsGamepadButtonPressed(0, player_inputs[0]->action_RIGHT) || input_manager->axis_debounce(0, player_inputs[0]->action_a_X, 0.5f))
        {
            menu_selection = (menu_selection + 1) % menu_selection_count;
        }
        if (IsKeyPressed(player_inputs[0]->action_k_LEFT) || IsGamepadButtonPressed(0, player_inputs[0]->action_LEFT) || input_manager->axis_debounce(0, player_inputs[0]->action_a_X, -0.5f))
        {
            menu_selection = (menu_selection - 1 + menu_selection_count) % menu_selection_count;
        }

        // player wide inputs
        for (int i = 0; i < game_data->player_count; i++)
        {

            int player_i_i = input_manager->player[i];

            // change color forward
            if (IsKeyPressed(player_inputs[i]->action_k_UP) || IsGamepadButtonPressed(player_i_i, player_inputs[i]->action_UP) || input_manager->axis_debounce(player_i_i, player_inputs[i]->action_a_Y, 0.5f))
            {
                color_selection = (1 + color_selection) % SHIP_COLORS;
                ships[i]->ship_body->set_color(ships[i]->ship_body, color_selection);
            }

            // change color backward
            if (IsKeyPressed(player_inputs[i]->action_k_DOWN) || IsGamepadButtonPressed(player_i_i, player_inputs[i]->action_DOWN) || input_manager->axis_debounce(player_i_i, player_inputs[i]->action_a_Y, -0.5f))
            {
                color_selection = (color_selection - 1 + SHIP_COLORS) % SHIP_COLORS;
                ships[i]->ship_body->set_color(ships[i]->ship_body, color_selection);
            }

            // remove player
            if (i != 0 && input_manager->player_mapped[i])
            {
                if (IsKeyPressed(player_inputs[i]->action_k_ESCAPE) || IsGamepadButtonPressed(player_i_i, player_inputs[i]->action_B))
                {
                    game_data->player_count--;
                    input_manager->unmap_player_input(i); // remove the input mapping for the player and shift the players down

                    change_player_count(&game_data->player_count); // this will reset the player count for all ships, recalculate segments, and set the shield

                    TraceLog(LOG_INFO, "Player %d removed from embarking", i);
                }
            }
        }
    }
}

static void scene_render(void)
{
    DrawTexture(*embark_room, 0, 0, WHITE);

    switch (menu_selection)
    {
    case 0:
        DrawTexture(*floor_dock, 22, 166, WHITE);
        break;
    case 1:
        DrawTexture(*floor_simulation, 214, 166, WHITE);
        break;

    default:
        DrawTexture(*floor_dock, 22, 166, WHITE);
        break;
    }

    DrawTexture(*embark_projector_beams, 51, 36, WHITE);
    DrawTexture(*holo_spots, 48, 13, WHITE);

    DrawTexture(*holo_beam, 114, 42, WHITE);
    DrawTexture(*embark_text_backing, 114, 51, WHITE);

    int num_x_positions[] = {133, 185, 81, 236};
    for (int i = 0; i < game_data->player_count; i++)
    {
        ships[i]->render(ships[i]);
        DrawTexture(*nums[i], num_x_positions[i], 148, WHITE);
    }
}

static void scene_cleanup(void)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        ships[i]->cleanup(ships[i]);
    }
}

Scene *create_embark_scene(void)
{
    input_manager = get_input_manager();

    // set textures
    floor_dock = &resource_manager.get_texture("embark-floor-dock")->texture;
    floor_simulation = &resource_manager.get_texture("embark-floor-simulation")->texture;
    holo_beam = &resource_manager.get_texture("embark-holo-beam")->texture;
    holo_spots = &resource_manager.get_texture("embark-holo-spots")->texture;
    nums[0] = &resource_manager.get_texture("embark-num-1")->texture;
    nums[1] = &resource_manager.get_texture("embark-num-2")->texture;
    nums[2] = &resource_manager.get_texture("embark-num-3")->texture;
    nums[3] = &resource_manager.get_texture("embark-num-4")->texture;
    embark_projector_beams = &resource_manager.get_texture("embark-projector-beams")->texture;
    embark_room = &resource_manager.get_texture("embark-room")->texture;
    embark_text_backing = &resource_manager.get_texture("embark-text-backing")->texture;

    game_data = create_game_data();

    embark_scene.init = scene_init;
    embark_scene.update = scene_update;
    embark_scene.render = scene_render;
    embark_scene.cleanup = scene_cleanup;

    return &embark_scene;
}
