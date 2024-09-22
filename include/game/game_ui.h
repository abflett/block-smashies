#ifndef GAME_UI
#define GAME_UI

#include "raylib.h"

typedef struct GameUi
{
    Texture2D *background; // Background texture for the game
    Texture2D *foreground;
    void (*render_before_content)(void);
    void (*render_after_content)(void);
    void (*update)(float delta_time);
    void (*cleanup)(void);
} GameUi;

GameUi *create_game_ui(void);

#endif