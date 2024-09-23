#include <stdarg.h>
#include <string.h>
#include "raylib.h"
#include "game_over_state.h"
#include "scene_manager.h"
#include "game_state_manager.h"
#include "high_score.h"
#include "game_status.h"

#define MAX_NAME_LENGTH 20

static GameState game_over_state;

static HighScore high_scores[10];
static int count;
static int score;
static char player_name[MAX_NAME_LENGTH + 1] = "Player"; // +1 for the null terminator
static bool adding_player_score;

static void update_player_score(float delta_time)
{
    int key = GetCharPressed();

    // Check if a character was pressed and if the name length is within the limit
    while (key > 0)
    {
        // NOTE: Only allow printable characters
        if ((key >= 32) && (key <= 125) && (strlen(player_name) < MAX_NAME_LENGTH))
        {
            int len = (int)strlen(player_name);
            player_name[len] = (char)key;
            player_name[len + 1] = '\0'; // Add null terminator
        }

        key = GetCharPressed(); // Check for more characters pressed
    }

    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE) && strlen(player_name) > 0)
    {
        player_name[strlen(player_name) - 1] = '\0';
    }

    // Check if the user presses Enter to confirm their name
    if (IsKeyPressed(KEY_ENTER))
    {
        add_high_score(high_scores, &count, player_name, score);
        save_high_scores("high_scores.json", high_scores, count);

        adding_player_score = false;
    }
}

static void state_init(void)
{
    // score = game_state_manager.context->game_status->currency;

    // Load high scores from file
    load_high_scores("high_scores.json", high_scores, &count);
    // Determine if the player's score is high enough to be added
    adding_player_score = is_high_score(high_scores, count, game_state_manager.context->game_status->score);
}

static void state_update(float delta_time)
{
    if (adding_player_score)
    {
        update_player_score(delta_time);
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            scene_manager.change(scene_manager.scenes.logo);
        }
    }
}

static void state_render(void)
{
    if (adding_player_score)
    {
        DrawText("Enter your name:", 5, 5, 8, LIGHTGRAY);
        DrawText(player_name, 5, 10 + 5, 8, WHITE); // Render the current inputted name
    }
    else
    {
        char high_score_text[50];
        for (int i = 0; i < count; ++i)
        {
            snprintf(high_score_text, sizeof(high_score_text), "%.38s - %d", high_scores[i].username, high_scores[i].score);
            DrawText(high_score_text, 5, 10 * i + 5, 8, LIGHTGRAY);
        }
    }
}

static void state_cleanup(void)
{
}

GameState *create_game_over_state(void)
{
    game_over_state.init = state_init;
    game_over_state.update = state_update;
    game_over_state.render = state_render;
    game_over_state.cleanup = state_cleanup;

    return &game_over_state;
};
