#ifndef SHIP_BODY_H
#define SHIP_BODY_H

#include "box2d/box2d.h"
#include "resource_manager.h"

typedef struct ShipBody
{
    int *segments;
    int *ship_color;
    b2Vec2 *position;
    Subtexture *subtexture;

    void (*set_segments)(struct ShipBody *ship_body, int segments);
    void (*set_color)(struct ShipBody *ship_body, int color);
    void (*render)(struct ShipBody *ship_body); // render based on segments and color
    void (*cleanup)(struct ShipBody *ship_body);
} ShipBody;

ShipBody *create_ship_body(int *segments, int *ship_color, b2Vec2 *position);

#endif