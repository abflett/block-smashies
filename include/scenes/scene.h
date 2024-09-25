#ifndef SCENE_H
#define SCENE_H

#include <stdarg.h>

typedef struct Scene
{
    void (*init)(int arg_count, va_list args);
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} Scene;

#endif
