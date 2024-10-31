#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdarg.h>

typedef struct GameState
{
    void (*init)(const int arg_count, va_list args);
    void (*update)(const float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} GameState;

#endif
