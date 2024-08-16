#include "raylib.h"
#include "game_over_state.h"
#include "scene_manager.h"
#include "high_score.h"
#include "game_status.h"

static HighScore high_scores[10];
static int count = 0;

static void state_init(void)
{
    // Load high scores from file
    load_high_scores("high_scores.json", high_scores, &count);

    // Add a new high score
    add_high_score(high_scores, &count, "Player 1", 1);

    // Save high scores to file
    save_high_scores("high_scores.json", high_scores, count);
}

static void state_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.change(scene_manager.scenes.logo);
    }
}

static void state_render(void)
{
    char high_score_text[50];
    for (int i = 0; i < count; ++i)
    {
        snprintf(high_score_text, sizeof(high_score_text), "%.38s - %.8d", high_scores[i].username, high_scores[i].score);
        DrawText(high_score_text, 5, 10 * i + 16, 8, LIGHTGRAY);
    }
}

static void state_cleanup(void)
{
}


GameState game_over_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};