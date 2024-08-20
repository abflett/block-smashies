#ifndef BALL_H
#define BALL_H

#include <stdbool.h>
#include "raylib.h"
#include "player.h"

struct Entities;

typedef struct Ball
{
    Texture2D texture;
    Vector2 position;
    Vector2 velocity;
    float radius;
    float *speed_multiplier;
    int *power;
    float *phase_nova;
    float *super_nova;
    bool active;
    void (*update)(struct Ball *ball, struct Entities *entities, float delta_time);
    void (*reset)(struct Ball *ball, Vector2 initial_position);
    void (*render)(struct Ball *ball);
} Ball;

Ball create_ball(Player *player, Vector2 position, Vector2 velocity);

#endif
