#ifndef PLAYING_UI_BARS_H
#define PLAYING_UI_BARS_H

#include "raylib.h"
#include "resource_manager.h"
#include "game_status.h"

typedef struct PlayingUiBar
{
    int player_index;
    int max_bar_level;
    int current_bar_level;
    float *stat; // Pointer to the player's stat (e.g., energy)
    float rotation;
    Vector2 origin;
    Vector2 position;
    Subtexture *subtexture;
} PlayingUiBar;

typedef struct PlayingUiBars
{
    PlayingUiBar playing_ui_bars[16];
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} PlayingUiBars;

PlayingUiBars *create_playing_ui_bars(GameStatus *game_status);

#endif