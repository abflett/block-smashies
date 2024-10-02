#ifndef ANIMATION_HANDLER_H
#define ANIMATION_HANDLER_H

#include <stdbool.h>
#include "raylib.h"
#include "resource.h"

typedef enum
{
    ANIMATION_LOOP,
    ANIMATION_ONCE,
    ANIMATION_PING_PONG
} AnimationType;

typedef struct AnimationHandler
{
    Animation *animation;
    AnimationType animation_type;
    int frame_index;
    int frame_count;
    float elapsed_time;
    float frame_time;
    bool ping_pong_forward;
    float rotation;
    bool is_playing;
    void (*update)(struct AnimationHandler *animation_handler, float delta_time, float rotation);
    void (*render)(struct AnimationHandler *animation_handler, Vector2 position);
    void (*cleanup)(struct AnimationHandler *animation_handler);

} AnimationHandler;

AnimationHandler *create_animation_manager(const char *id, AnimationType animation_type, float frame_time);

#endif