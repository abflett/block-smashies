#include "raylib.h"
#include "settings_menu_state.h"
#include "scene_manager.h"
#include "game_state_manager.h"
#include "high_score.h"
#include "game_status.h"
#include "resource_manager.h"
#include "input_manager.h"
#include "settings.h"

#define MAX_NAME_LENGTH 14

static GameState settings_menu_state;
static GameData *game_data;
static InputManager *input_manager;
static Font *font;

static float min_scene_time;

static void state_init(void)
{
    min_scene_time = settings.game.min_game_over_screen_time;
    game_data = game_state_manager.context->game_data;
}

static void state_update(float delta_time)
{
    if (min_scene_time >= 0)
    {
        min_scene_time -= delta_time;
    }

    for (int i = 0; i < game_data->player_count; i++)
    {
        InputMapping *input = input_manager->get_player_input(i);
        int mapping = input_manager->player[i];

        if (input_manager->key_debounce(mapping, input->action_k_ENTER) || //
            input_manager->button_debounce(mapping, input->action_A) ||
            input_manager->button_debounce(mapping, input->action_B) ||
            input_manager->button_debounce(mapping, input->action_START) && min_scene_time <= 0.0f)
        {
            game_state_manager.change(game_state_manager.states.playing);
        }
    }
}

static void state_render(void)
{
    const char *text = "Settings Menu";
    Vector2 text_size = MeasureTextEx(*font, text, 14, 0.0f);
    Vector2 text_position = {
        (settings.game.target_size.x - text_size.x) / 2,
        (settings.game.target_size.y - text_size.y) / 2};
    DrawTextEx(*font, text, text_position, 14, 0.0f, WHITE);
}

static void state_cleanup(void)
{
}

GameState *create_settings_menu_state(void)
{
    font = resource_manager.get_pixel7_font();
    input_manager = get_input_manager();

    settings_menu_state.init = state_init;
    settings_menu_state.update = state_update;
    settings_menu_state.render = state_render;
    settings_menu_state.cleanup = state_cleanup;

    return &settings_menu_state;
};