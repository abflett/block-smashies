#ifndef SHIP_H
#define SHIP_H

#include <stdbool.h>
#include "box2d/box2d.h"
#include "ship_body.h"
#include "ship_shield.h"
#include "ship_thrusters.h"
#include "game_data.h"

struct GameContext;

typedef struct Ship
{
    EntityType type;
    bool active;
    bool physics_active;
    b2BodyId body;
    b2BodyId constraint;

    ShipBody *ship_body;
    ShipShield *ship_shield;
    ShipThrusters *ship_thrusters;

    int segments;
    int *player;       // set from game_data
    int *player_count; // set from game_data
    int previous_player_count;

    b2Vec2 position;
    b2Vec2 velocity;
    b2Vec2 shield_size;

    float pulse_timer;
    float pulse_active_timer;
    float boost_active_timer;

    // set from game data
    float *ship_force;     // general movement ship_force
    float *orb_control;    // ball manipulation
    float *ship_damping;   // de-acceleration - affects max velocity as well
    float *max_energy;     // max_energy
    float *boost_force;    // boost ship_force - horizontal burst
    float *boost_cooldown; // boost cooldown timer < is better
    float *pulse_force;    // boost ship_force - vertical burst
    float *pulse_cooldown; // pulse cooldown timer < is better
    float *heat;           // heat buildup % < is no heat
    float *phase_shift;
    float *orb_shot;
    int *ship_color; // from game_data
    int *shield_strength;

    // ship input commands
    void (*move_ship)(const struct Ship *ship, const int direction);
    void (*boost_ship)(struct Ship *ship, const int direction);
    void (*pulse_ship)(struct Ship *ship);

    void (*activate_ship_physics)(struct Ship *ship, struct GameContext *game_context);
    int (*calculate_segments)(struct Ship *ship);
    void (*move)(struct Ship *ship, const b2Vec2 position, const b2Vec2 velocity); // movement for animation scenes without physics
    void (*update)(struct Ship *ship, const float delta_time);               // handle player input and other updates
    void (*render)(struct Ship *ship);
    void (*disable)(struct Ship *ship);                // set active false
    void (*reset)(struct Ship *ship, const b2Vec2 position); // reset position on death etc.
    void (*cleanup)(struct Ship *ship);
} Ship;

Ship *create_ship(int *player, GameData *game_data, const b2Vec2 position);

#endif