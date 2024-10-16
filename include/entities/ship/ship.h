#ifndef SHIP_H
#define SHIP_H

#include <stdbool.h>
#include "box2d/box2d.h"
#include "ship_body.h"
#include "ship_shield.h"
#include "ship_thrusters.h"
#include "game_data.h" // add later

// struct ShipBody;
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
    int *player;       // from game_data
    int *player_count; // from game_data
    int previous_player_count;

    b2Vec2 position;
    b2Vec2 velocity;
    b2Vec2 shield_size;

    float pulse_timer;
    float pulse_active_timer;
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

    int *ship_color; // from game_data
    int *shield_level;

    // ship input commands
    void (*move_ship)(struct Ship *ship, int direction);
    void (*boost_ship)(struct Ship *ship, int direction);
    void (*pulse_ship)(struct Ship *ship);

    void (*activate_ship_physics)(struct Ship *ship, struct GameContext *game_context);
    int (*calculate_segments)(struct Ship *ship);
    void (*move)(struct Ship *ship, b2Vec2 position, b2Vec2 velocity); // movement for animation scenes without physics
    void (*update)(struct Ship *ship, float delta_time);               // handle player input and other updates
    void (*render)(struct Ship *ship);
    void (*disable)(struct Ship *ship);                // set active false
    void (*reset)(struct Ship *ship, b2Vec2 position); // reset position on death etc.
    void (*cleanup)(struct Ship *ship);
} Ship;

Ship *create_ship(int *player, GameData *game_data, b2Vec2 position);

#endif