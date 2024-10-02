#ifndef SHIP_H
#define SHIP_H

#include <stdbool.h>
#include "ship_body.h"
#include "ship_shield.h"
#include "ship_thrusters.h"
// #include "game_data.h" // add later

struct ShipBody;

typedef struct Ship
{
    bool active;

    struct ShipBody *ship_body;
    ShipShield *ship_shield;
    ShipThrusters *ship_thrusters;

    int segments;
    int player;       // from game_data
    int player_count; // from game_data
    b2Vec2 position;
    int ship_color; // from game_data

    void (*move)(b2Vec2 position);    // movement for animation scenes without physics
    void (*update)(float delta_time); // handle player input and other updates
    void (*render)(void);
    void (*disable)(void);          // set active false
    void (*reset)(b2Vec2 position); // reset position on death etc.
    void (*cleanup)(void);
} Ship;

Ship *create_ship(int player, int player_count, int ship_color, b2Vec2 position);

#endif