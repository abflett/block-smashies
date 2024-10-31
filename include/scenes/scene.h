#ifndef SCENE_H
#define SCENE_H

#include <stdarg.h>

typedef struct Scene
{
    void (*init)(const int arg_count, va_list args);
    void (*update)(const float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} Scene;

#endif
