#ifndef BALL_H
#define BALL_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "player.h"
#include "entity_type.h"

struct Entities;

typedef struct Ball
{
    EntityType type;
    b2BodyId body;
    bool active;

    Texture2D *texture;
    float radius;

    int *power;
    float *phase_nova;
    float *super_nova;
    float *max_speed;

    void (*update)(struct Ball *ball, struct Entities *entities, float delta_time);
    void (*render)(struct Ball *ball);
    void (*clean_up)(struct Ball *ball);
} Ball;

Ball create_ball(Player *player, b2WorldId world_id, b2Vec2 position, b2Vec2 velocity);

#endif
