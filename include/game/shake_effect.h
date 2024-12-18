#ifndef SHAKE_EFFECT_H
#define SHAKE_EFFECT_H

#include "raylib.h"

typedef struct ShakeEffect
{
    float intensity; // Current shake intensity
    float duration;  // Total duration of the shake
    float elapsed;   // Time elapsed since the shake started
    Vector2 offset;  // Current offset for rendering

    void (*start)(const float intensity, const float duration);
    void (*update)(const float delta_time);
} ShakeEffect;

ShakeEffect *create_shake_effect(void);
ShakeEffect *get_shake_effect(void);

#endif