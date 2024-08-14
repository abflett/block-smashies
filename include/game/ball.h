#ifndef BALL_H
#define BALL_H

#include "raylib.h"

typedef struct Ball
{
    Texture2D texture;
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed_multiplier;
    void (*update)(struct Ball *ball, float delta_time);
    void (*reset)(struct Ball *ball, Vector2 initial_position);
    void (*render)(struct Ball *ball);
} Ball;

Ball create_ball(Vector2 initial_position);

#endif
