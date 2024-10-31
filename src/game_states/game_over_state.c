#include "raylib.h"
#include "game_over_state.h"
#include "game_state_manager.h"
#include "resource_manager.h"
#include "input_manager.h"
#include "settings.h"

#define MAX_NAME_LENGTH 14

static GameState game_over_state;
static GameData *game_data;
static InputManager *input_manager;
static Font *font;

static float min_scene_time;

static void state_init(const int arg_count, va_list args)
{
    min_scene_time = settings.game.min_screen_time;
    game_data = game_state_manager.context->game_data;
}

static void state_update(const float delta_time)
{
    if (min_scene_time >= 0)
    {
        min_scene_time -= delta_time;
    }

    for (int i = 0; i < game_data->player_count; i++)
    {
        const InputMapping *input = input_manager->get_player_input(i);
        const int mapping = input_manager->player[i];

        if (input_manager->key_debounce(mapping, input->action_k_ENTER) || //
            input_manager->button_debounce(mapping, input->action_A) ||
            input_manager->button_debounce(mapping, input->action_B) ||
            input_manager->button_debounce(mapping, input->action_START) && min_scene_time <= 0.0f)
        {
            game_state_manager.change(game_state_manager.states.high_score, 0, NULL);
        }
    }
}

static void state_render(void)
{
    const char *text = "Game Over";
    const Vector2 text_size = MeasureTextEx(*font, text, 14, 0.0f);
    const Vector2 text_position = {
        (settings.game.target_size.x - text_size.x) / 2,
        (settings.game.target_size.y - text_size.y) / 2};
    DrawTextEx(*font, text, text_position, 14, 0.0f, WHITE);
}

static void state_cleanup(void)
{
}

GameState *create_game_over_state(void)
{
    font = resource_manager.get_pixel7_font();
    input_manager = get_input_manager();

    game_over_state.init = state_init;
    game_over_state.update = state_update;
    game_over_state.render = state_render;
    game_over_state.cleanup = state_cleanup;

    return &game_over_state;
};
