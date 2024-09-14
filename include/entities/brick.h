#ifndef BRICK_H
#define BRICK_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "entity_type.h"
#include "resource.h"

struct Entities;

typedef enum
{
    DK_BROWN_BRICK
} BrickColor;

typedef struct Brick
{
    EntityType type;
    b2BodyId body;
    bool active;

    Subtexture *subtexture;
    b2Vec2 size;
    float health;
    float max_health;
    BrickColor color;

    void (*render)(struct Brick *brick);
    void (*clean_up)(struct Brick *brick);
    void (*reset)(struct Brick *brick, b2Vec2 position, float health, BrickColor color);
    void (*disable)(struct Brick *brick);
} Brick;

Brick *create_brick(b2WorldId world_id, b2Vec2 position, float health, BrickColor color);

#endif
