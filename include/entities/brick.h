#ifndef BRICK_H
#define BRICK_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "entity_type.h"

struct Entities;

typedef struct Brick
{
    EntityType type;
    b2BodyId body;
    bool active;

    Texture2D *texture;
    b2Vec2 size;
    int health;

    void (*render)(struct Brick *brick);
    void (*clean_up)(struct Brick *brick);
    void (*reset)(struct Brick *brick, b2Vec2 position, int health);
    void (*disable)(struct Brick *brick);
} Brick;

Brick *create_brick(b2WorldId world_id, b2Vec2 position, int health);

#endif
