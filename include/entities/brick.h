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
    BRICK_DK_BROWN
} BrickType;

typedef struct Brick
{
    EntityType type;
    b2BodyId body;
    bool active;

    Subtexture *subtexture;
    b2Vec2 size;
    float health;
    float max_health;
    BrickType brick_type;

    void (*update)(struct Brick *brick, float delta_time);
    void (*render)(struct Brick *brick);
    void (*clean_up)(struct Brick *brick);
    void (*reset)(struct Brick *brick, b2Vec2 position, BrickType color);
    void (*disable)(struct Brick *brick);
} Brick;

Brick *create_brick(b2WorldId world_id, b2Vec2 position, BrickType color);

#endif
