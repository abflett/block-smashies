#ifndef SHIP_BODY_H
#define SHIP_BODY_H

#include "box2d/box2d.h"
#include "resource_manager.h"

typedef struct ShipBody
{
    int *segments;
    int *ship_color;
    b2Vec2 *position;
    Subtexture *shield_subtexture;

    void (*render)(void); // render based on segments and color
    void (*cleanup)(void);
} ShipBody;

ShipBody *create_ship_body(int *segments, int *ship_color, b2Vec2 *position);

#endif