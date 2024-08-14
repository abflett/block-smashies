#include <stdio.h>
#include "raylib.h"
#include "game_status.h"

// Update game status (e.g., increment game time)
void update_game_status(GameStatus *status, float delta_time)
{
    status->game_time += delta_time;
}

// Reset game status to initial values
void reset_game_status(GameStatus *status)
{
    status->score = 0;
    status->lives = 3;
    status->game_time = 0.0f;
}

// Render game status on screen
void render_game_status(const GameStatus *status)
{
    char score_text[20];
    snprintf(score_text, sizeof(score_text), "Score: %d", status->score);
    DrawText(score_text, 5, 5, 8, LIGHTGRAY);

    char time_text[20];
    int minutes = (int)(status->game_time / 60);
    float seconds = status->game_time - minutes * 60;
    snprintf(time_text, sizeof(time_text), "Time: %d:%04.2f", minutes, seconds);
    DrawText(time_text, 145, 5, 8, LIGHTGRAY);

    char lives_text[20];
    snprintf(lives_text, sizeof(lives_text), "Lives: %d", status->lives);
    DrawText(lives_text, 295, 5, 8, LIGHTGRAY);
}

// Initialize game status
GameStatus create_game_status(void)
{
    GameStatus status;
    status.score = 0;
    status.lives = 3;
    status.game_time = 0.0f;
    status.update = update_game_status;
    status.reset = reset_game_status;
    status.render = render_game_status;
    return status;
}