#ifndef SHIP_SHIELD_H
#define SHIP_SHIELD_H

#include "box2d/box2d.h"
#include "resource_manager.h"
#include "entity_type.h"

typedef struct ShipShield
{
    int *segments;
    int previous_segments;
    int *shield_strength;
    int previous_shield_level;
    b2Vec2 *position;
    Subtexture *subtexture;

    void (*render)(struct ShipShield *ship_shield);
    void (*cleanup)(struct ShipShield *ship_shield);
} ShipShield;

ShipShield *create_ship_shield(int *segments, int *shield_strength, b2Vec2 *position);

#endif