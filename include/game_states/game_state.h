#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct GameState
{
    void (*init)(void);
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} GameState;

#endif