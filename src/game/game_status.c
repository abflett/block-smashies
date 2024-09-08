#include <stdio.h>
#include "raylib.h"
#include "game_status.h"

static void update_game_status(GameStatus *status, float delta_time)
{
    status->game_time += delta_time;
}

static void reset_game_status(GameStatus *status)
{
    status->score = 0;
    status->lives = 3;
    status->game_time = 0.0f;
}

static void render_game_status(const GameStatus *status)
{
    char time_text[21];
    int minutes = (int)(status->game_time / 60);
    float seconds = status->game_time - minutes * 60;
    snprintf(time_text, sizeof(time_text), "%d:%04.2f", minutes, seconds);
    DrawText(time_text, 18, 14, 8, LIGHTGRAY);

    char score_text[21];
    snprintf(score_text, sizeof(score_text), "%d", status->score);
    DrawText(score_text, 19, 28, 8, LIGHTGRAY);

    char lives_text[21];
    snprintf(lives_text, sizeof(lives_text), "<3 %d", status->lives);
    DrawText(lives_text, 19, 42, 8, LIGHTGRAY);
}

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