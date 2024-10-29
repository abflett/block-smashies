#ifndef SHIP_THRUSTER_H
#define SHIP_THRUSTER_H

#include "animation_handler.h"
#include "box2d/box2d.h"

typedef struct ShipThrusters
{
    int *segments;
    b2Vec2 *position;

    int *left_level;
    int *right_level;
    int *bottom_level;

    AnimationHandler *left_thruster;
    AnimationHandler *right_thruster;
    AnimationHandler *bottom_thruster; // render more then once for multiple segments

    // function to control what thruster level and direction to activate, right, left and boost up
    void (*render)(const struct ShipThrusters *ship_thrusters);  // render thrusters based on segments and postion pointers
    void (*cleanup)(struct ShipThrusters *ship_thrusters); // cleanup animation handlers
} ShipThrusters;

ShipThrusters *create_ship_thrusters(int *segments, b2Vec2 *position);

#endif