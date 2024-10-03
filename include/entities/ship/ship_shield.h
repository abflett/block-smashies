#ifndef SHIP_SHIELD_H
#define SHIP_SHIELD_H

#include "box2d/box2d.h"
#include "raylib.h"
#include "entity_type.h"
#include "resource_manager.h"

typedef struct ShipShield
{
    int *segments;
    int *shield_level;
    b2Vec2 *position;
    Subtexture *subtexture;

    void (*set_shield)(struct ShipShield *ship_shield);
    void (*render)(struct ShipShield *ship_shield);
    void (*cleanup)(struct ShipShield *ship_shield);
} ShipShield;

ShipShield *create_ship_shield(int *segments, int *shield_level, b2Vec2 *position);

#endif