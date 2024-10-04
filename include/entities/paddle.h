#ifndef PADDLE_H
#define PADDLE_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "game_data.h"
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

    float *force;          // general movement force
    float *friction;       // ball manipulation
    float *damping;        // de-acceleration - affects max velocity as well
    float *max_energy;     // max_energy
    float *boost_force;    // boost force - horizontal burst
    float *boost_cooldown; // boost cooldown timer < is better
    float *pulse_force;    // boost force - vertical burst
    float *pulse_cooldown; // pulse cooldown timer < is better
    float *heat;           // heat buildup % < is no heat

    float *phase_shift;
    bool *orb_shot;

    void (*update)(struct Paddle *paddle, float delta_time);
    void (*render)(struct Paddle *paddle);
    void (*clean_up)(struct Paddle *paddle);
    void (*reset)(struct Paddle *paddle, int player_num);
    void (*disable)(struct Paddle *paddle);
} Paddle;

Paddle *create_paddle(int player_num, GameData *game_data, b2WorldId world_id);

#endif
