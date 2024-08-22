#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdarg.h>

typedef struct GameState
{
    void (*init)(int argc, va_list args);
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} GameState;

#endif
