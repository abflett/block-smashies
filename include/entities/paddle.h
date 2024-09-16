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
    float pulse_timer;
    float pulse_active_timer;
    float boost_timer_left;
    float boost_timer_right;
    float boost_active_timer;

    float *acceleration;
    float *max_velocity;
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
    void (*reset)(struct Paddle *paddle, int player_num);
    void (*disable)(struct Paddle *paddle);
} Paddle;

Paddle *create_paddle(int player_num, Player *player, b2WorldId world_id);

#endif
