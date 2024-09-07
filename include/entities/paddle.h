#ifndef PADDLE_H
#define PADDLE_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "player.h"
#include "entity_type.h"

typedef struct Paddle
{
    EntityType type;
    b2BodyId body;
    b2BodyId constraint;
    bool active;

    Texture2D *texture;
    b2Vec2 size;

    int player_num;
    float force_timer;
    float force_active_timer;

    float *acceleration;
    float *max_speed;
    float *friction;
    float *charge;
    float *booster_str;
    float *pulse_str;
    float *phase_shift;
    bool *time_manipulation;
    bool *orb_shot;

    void (*update)(struct Paddle *paddle, float delta_time);
    void (*render)(struct Paddle *paddle);
    void (*clean_up)(struct Paddle *paddle);
} Paddle;

Paddle create_paddle(int player_num, Player *player, b2WorldId world_id);

#endif
