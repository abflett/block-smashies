#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "game_status.h"
#include "resource_manager.h"
#include "settings.h"

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
    DrawTexture(resource_manager.get_texture("clock-ui")->texture, 18, 15, WHITE);
    DrawTextEx(*resource_manager.get_pixel7_font(), time_text, (Vector2){26.0f, 14.0f}, 7, 0, settings.game.screen_text_color);

    char currency_text[21];
    snprintf(currency_text, sizeof(currency_text), "%d", status->currency);
    DrawTexture(resource_manager.get_texture("nanite-ui")->texture, 18, 30, WHITE);
    DrawTextEx(*resource_manager.get_pixel7_font(), currency_text, (Vector2){26.0f, 29.0f}, 7, 0, settings.game.screen_text_color);

    for (int i = 0; i < status->lives; i++)
    {
        DrawTexture(resource_manager.get_texture("life-ui")->texture, (i * 8) + 19, 45, WHITE);
    }
}

GameStatus create_game_status(void)
{
    GameStatus status;
    status.score = 0;
    status.currency = 0;
    status.lives = 3;
    status.game_time = 0.0f;
    status.game_over = false;
    status.is_hold = true;
    status.is_pause = false;
    status.update = update_game_status;
    status.reset = reset_game_status;
    status.render = render_game_status;
    return status;
}