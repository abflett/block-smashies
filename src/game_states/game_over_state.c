#include "raylib.h"
#include "game_over_state.h"
#include "scene_manager.h"
#include "game_state_manager.h"
#include "high_score.h"
#include "game_status.h"
#include "resource_manager.h"
#include "game.h"

#define MAX_NAME_LENGTH 14

static GameState game_over_state;
static GameData *game_data;
static InputManager *input_manager;
static Font *font;

static HighScore high_scores[10];
static int count;
static int score;

static void state_init(void)
{
    game_data = game_state_manager.context->game_data;
    score = game_state_manager.context->game_status->score;

    // Load high scores from file
    load_high_scores(high_scores, &count);

    // Determine if the player's score is high enough to be added
    if (is_high_score(high_scores, count, score))
    {
        add_high_score(high_scores, &count, game_data->name, score);
        save_high_scores(high_scores, count);
    }
}

static void state_update(float delta_time)
{
    for (int i = 0; i < game_data->player_count; i++)
    {
        InputMapping *input = input_manager->get_player_input(i);
        int mapping = input_manager->player[i];

        if (input_manager->key_debounce(mapping, input->action_k_ENTER) || //
            input_manager->button_debounce(mapping, input->action_A) ||
            input_manager->button_debounce(mapping, input->action_B) ||
            input_manager->button_debounce(mapping, input->action_START))
        {
            scene_manager.change(scene_manager.scenes.logo, 0);
        }
    }
}

static void state_render(void)
{
    for (int i = 0; i < count; ++i)
    {
        const char *high_score_text = TextFormat("%s - %d", high_scores[i].username, high_scores[i].score);
        DrawTextEx(*font, high_score_text, (Vector2){5, (float)10 * i + 5}, 7, 0, LIGHTGRAY);
    }
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
