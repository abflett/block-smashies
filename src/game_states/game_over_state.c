#include "raylib.h"
#include "game_over_state.h"
#include "scene_manager.h"
#include "logo_scene.h"
#include "high_score.h"

HighScore high_scores[10];
int count = 0;

GameState game_over_state = {
    .init = game_over_state_init,
    .update = game_over_state_update,
    .render = game_over_state_render,
    .cleanup = game_over_state_cleanup,
};

void game_over_state_init(void)
{
    // Load high scores from file
    load_high_scores("high_scores.json", high_scores, &count);

    // Add a new high score
    add_high_score(high_scores, &count, "Adam", 1000000);

    // Save high scores to file
    save_high_scores("high_scores.json", high_scores, count);
}

void game_over_state_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.change(&logo_scene);
    }
}

void game_over_state_render(void)
{
    char high_score_text[50];
    for (int i = 0; i < count; ++i)
    {
        snprintf(high_score_text, sizeof(high_score_text), "%.38s - %.8d", high_scores[i].username, high_scores[i].score);
        DrawText(high_score_text, 5, 10 * i + 16, 8, LIGHTGRAY);
    }
}

void game_over_state_cleanup(void)
{
}
