#include <stdbool.h>
#include "raylib.h"
#include "embark_scene.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "game_data.h"
#include "ship.h"
#include "virtual_keyboard.h"


#define SHIP_COLORS 25
#define MAX_PLAYERS 4
#define MAX_NAME_LENGTH 14

static Scene embark_scene;
static InputManager *input_manager;
static VirtualKeyboard *virtual_keyboard;
static GameData *game_data;
static Ship *ships[MAX_PLAYERS];

static Font *font;
static Vector2 text_position = {160, 52};

static Texture2D *floor_dock;
static Texture2D *floor_simulation;
static Texture2D *holo_beam;
static Texture2D *holo_spots;
static Texture2D *nums[MAX_PLAYERS];
static Texture2D *embark_projector_beams;
static Texture2D *embark_room;
static Texture2D *embark_text_backing;

static bool is_loading;
static int menu_selection = 2;
static const int menu_selection_count = 3; // 0 = main menu, 1 = gameplay, 2 = change team name
// static bool menu_selection_mode = true;

static Color hologram_color = {255, 255, 255, 255};
static Color projector_beam_color = {255, 255, 255, 255};

static float hologram_beam_timer = 0.0f;

static void scene_init(const int arg_count, const va_list args)
{
    game_data = create_game_data();

    for (int i = 0; i < arg_count; i++)
    {
        if (i == 0)
            is_loading = (bool)va_arg(args, int);
    }

    if (is_loading)
    {
        // enable game_data_summaries - load existing game data
        TraceLog(LOG_INFO, "Reliving Past");
    }
    else
    {
        // initialize new game data - new game data
        TraceLog(LOG_INFO, "Embark Story");
    }

    menu_selection = 1; // Todo: change team name = 2

    game_data->player_count = 1;
    input_manager->reset_player_inputs();

    const float x_positions[] = {135, 188, 84, 239};
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (ships[i] == NULL)
        {
            ships[i] = create_ship(&game_data->ships[i].player_num,
                                   game_data,
                                   (b2Vec2){x_positions[i], 100});
        }
    }

    virtual_keyboard = create_virtual_keyboard(game_data->name, MAX_NAME_LENGTH, text_position, (Vector2){40, 90}, settings.colors.blue_04);
}

static void update_timers(const float delta_time)
{
    hologram_beam_timer += delta_time;
    if (hologram_beam_timer > 0.05f)
    {
        hologram_color = (Color){255, 255, 255, GetRandomValue(200, 255)};
        projector_beam_color = (Color){255, 255, 255, GetRandomValue(100, 155)};
        hologram_beam_timer = 0.0f;
    }
}

static void scene_update(const float delta_time)
{
    update_timers(delta_time);

    if (is_loading)
    {
        // update game_data_summaries inputs
        if (input_manager->key_debounce(0, KEY_ESCAPE) || input_manager->key_debounce(0, KEY_ENTER))
        {
            update_game_data("The UUID of the game data to load");
            is_loading = false;
        }
        return;
    }

    virtual_keyboard->update(virtual_keyboard, delta_time);
    if (virtual_keyboard->active)
    {
        return;
    }

    // set player inputs for easy access
    InputMapping *player_inputs[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        player_inputs[i] = input_manager->get_player_input(i);
    }

    // check for key presses to add players
    if (input_manager->check_for_new_players(game_data->player_count))
    {
        game_data->ships[game_data->player_count].active = input_manager->player_mapped[game_data->player_count];
        game_data->ships[game_data->player_count].ship_color = GetRandomValue(0, 24); // random ship color between 0-24 options
        game_data->player_count++;
    }

    // menu selection action
    if (input_manager->key_debounce(0, player_inputs[0]->action_k_ENTER) || IsGamepadButtonPressed(0, player_inputs[0]->action_A) || IsGamepadButtonPressed(0, player_inputs[0]->action_START))
    {
        switch (menu_selection)
        {
        case 0:
            scene_manager.change(scene_manager.scenes.main_menu, 0);
            break;
        case 1:
            scene_manager.change(scene_manager.scenes.gameplay, 2, game_data, ships);
            break;
        case 2:
            virtual_keyboard->activate(virtual_keyboard);
            break;
        default:
            scene_manager.change(scene_manager.scenes.main_menu, 0);
            break;
        }
    }

    // menu selection navigation
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
        // get player input mapping for easy access
        const int player_i_i = input_manager->player[i];

        // change color
        if (IsKeyPressed(player_inputs[i]->action_k_UP) || IsGamepadButtonPressed(player_i_i, player_inputs[i]->action_UP) || input_manager->axis_debounce(player_i_i, player_inputs[i]->action_a_Y, 0.5f))
        {
            game_data->ships[i].ship_color = (1 + game_data->ships[i].ship_color) % SHIP_COLORS;
        }
        if (IsKeyPressed(player_inputs[i]->action_k_DOWN) || IsGamepadButtonPressed(player_i_i, player_inputs[i]->action_DOWN) || input_manager->axis_debounce(player_i_i, player_inputs[i]->action_a_Y, -0.5f))
        {
            game_data->ships[i].ship_color = (game_data->ships[i].ship_color - 1 + SHIP_COLORS) % SHIP_COLORS;
        }

        // remove player
        if (i != 0 && input_manager->player_mapped[i])
        {
            if (IsKeyPressed(player_inputs[i]->action_k_ESCAPE) || IsGamepadButtonPressed(player_i_i, player_inputs[i]->action_B))
            {
                game_data->player_count--;
                input_manager->unmap_player_input(i); // remove the input mapping for the player and shift the players down
                game_data->ships[i].active = input_manager->player_mapped[i];

                // shift the ship colors down
                for (int j = i; j < game_data->player_count; j++)
                {
                    game_data->ships[j].ship_color = game_data->ships[j + 1].ship_color;
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
    case 2:
        // modify alpha color of projector beams
        break;
    default:
        DrawTexture(*floor_dock, 22, 166, WHITE);
        break;
    }

    DrawTexture(*embark_projector_beams, 51, 36, hologram_color);
    DrawTexture(*holo_spots, 48, 13, hologram_color);

    // Todo: use switch case for hologram beam color
    DrawTexture(*holo_beam, 114, 42, projector_beam_color);
    DrawTexture(*embark_text_backing, 114, 51, menu_selection == 2 ? hologram_color : projector_beam_color);

    const int num_x_positions[] = {133, 185, 81, 236};
    for (int i = 0; i < game_data->player_count; i++)
    {
        ships[i]->render(ships[i]);
        DrawTexture(*nums[i], num_x_positions[i], 148, WHITE);
    }

    const Vector2 text_size = MeasureTextEx(*font, game_data->name, 7, 0.0f);
    const Vector2 text_centered_position = {
        (float)(int)(text_position.x - text_size.x / 2), // Center horizontally
        text_position.y                                  // Keep vertical position the same
    };

    DrawTextEx(*font, game_data->name, text_centered_position, 7, 0.0f, menu_selection == 2 ? settings.colors.blue_05 : settings.colors.blue_04);

    if (is_loading)
    {
        // Todo: render game_data_summaries same setup as the virtual keyboard
        DrawRectangle(20, 20, 280, 140, BLACK);
    }

    virtual_keyboard->render(virtual_keyboard);
}

static void scene_cleanup(void)
{
    virtual_keyboard->cleanup(virtual_keyboard);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        ships[i]->cleanup(ships[i]);
        ships[i] = NULL;
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
    font = resource_manager.get_pixel7_font();

    embark_scene.init = scene_init;
    embark_scene.update = scene_update;
    embark_scene.render = scene_render;
    embark_scene.cleanup = scene_cleanup;

    return &embark_scene;
}
