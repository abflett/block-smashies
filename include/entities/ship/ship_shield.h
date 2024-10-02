#ifndef SHIP_SHIELD_H
#define SHIP_SHIELD_H

#include "box2d/box2d.h"
#include "raylib.h"
#include "entity_type.h"
#include "resource.h"

typedef struct ShipShield
{
    int *segments;
    int *shield_level;
    b2Vec2 *position;
    Subtexture *shield_subtexture;

    void (*render)(void);
    void (*cleanup)(void);
} ShipShield;

ShipShield *create_ship_shield(int *segments, int *shield_level, b2Vec2 *position);

#endif