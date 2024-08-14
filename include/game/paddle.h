#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

typedef struct Paddle
{
    Texture2D texture;
    Vector2 position;
    Vector2 size;
    float speed;
    float acceleration;
    float max_speed;
    float friction;
    void (*update)(struct Paddle *paddle, float delta_time);
    void (*reset)(struct Paddle *paddle);
    void (*render)(struct Paddle *paddle); // Corrected function signature
} Paddle;

Paddle create_paddle(void);

#endif
