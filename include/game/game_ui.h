#ifndef GAME_UI
#define GAME_UI

#include "raylib.h"
#include "game_status.h"

typedef struct GameUi
{
    void (*render_before_content)(void);
    void (*render_after_content)(void);
    void (*update)(float delta_time);
    void (*cleanup)(void);
} GameUi;

GameUi *create_game_ui(GameStatus *game_status);

#endif