#ifndef BALL_H
#define BALL_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "game_data.h"
#include "entity_type.h"

struct Entities;

typedef struct Ball
{
    EntityType type;
    b2BodyId body;
    bool active;

    Texture2D *texture;
    float radius;

    float *power;
    float *phase_nova;
    float *super_nova;
    float *max_velocity;
    float *min_velocity;

    void (*update)(struct Ball *ball, float delta_time);
    void (*render)(struct Ball *ball);
    void (*clean_up)(struct Ball *ball);
    void (*reset)(struct Ball *ball, b2Vec2 position, b2Vec2 velocity);
    void (*disable)(struct Ball *ball);
} Ball;

Ball *create_ball(GameData *player, b2WorldId world_id, b2Vec2 position, b2Vec2 velocity);

#endif
