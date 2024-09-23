#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "game_status.h"
#include "resource_manager.h"
#include "settings.h"

GameStatus status;

static void update_game_status(float delta_time)
{
    status.game_time += delta_time;
}

static void reset_game_status(void)
{
    status.score = 0;
    status.lives = 3;
    status.game_time = 0.0f;
}

GameStatus *create_game_status(void)
{
    status.score = 0;
    status.currency = 0;
    status.lives = 3;
    status.game_time = 0.0f;
    status.game_over = false;
    status.is_hold = true;
    status.is_pause = false;
    status.update = update_game_status;
    status.reset = reset_game_status;
    return &status;
}