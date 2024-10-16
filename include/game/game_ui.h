#ifndef GAME_UI
#define GAME_UI

#include "raylib.h"

struct GameContext;

typedef struct Debris
{
    float x;
    float y;
    float fall_speed;
} Debris;

typedef struct GameUi
{
    void (*render_before_content)(void);
    void (*render_after_content)(void);
    void (*update)(float delta_time);
    void (*cleanup)(void);
} GameUi;

GameUi *create_game_ui(struct GameContext *game_context);

#endif