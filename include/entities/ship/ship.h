#ifndef SHIP_H
#define SHIP_H

#include <stdbool.h>
#include "box2d/box2d.h"
#include "ship_body.h"
#include "ship_shield.h"
#include "ship_thrusters.h"
#include "game_data.h" // add later

struct ShipBody;
struct GameContext;

typedef struct Ship
{
    EntityType type;
    bool active;
    bool physics_active;
    b2BodyId body;
    b2BodyId shield;
    b2BodyId constraint;

    struct ShipBody *ship_body;
    ShipShield *ship_shield;
    ShipThrusters *ship_thrusters;

    int segments;
    int *player;       // from game_data
    int *player_count; // from game_data
    int previous_player_count;
    b2Vec2 position;
    int *ship_color; // from game_data
    int *shield_level;

    void (*activate_ship_physics)(struct Ship *ship, struct GameContext *game_context);
    int (*calculate_segments)(struct Ship *ship);
    void (*move)(struct Ship *ship, b2Vec2 position);    // movement for animation scenes without physics
    void (*update)(struct Ship *ship, float delta_time); // handle player input and other updates
    void (*render)(struct Ship *ship);
    void (*disable)(struct Ship *ship);                // set active false
    void (*reset)(struct Ship *ship, b2Vec2 position); // reset position on death etc.
    void (*cleanup)(struct Ship *ship);
} Ship;

Ship *create_ship(int *player, GameData *game_data, b2Vec2 position);

#endif