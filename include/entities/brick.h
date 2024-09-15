#ifndef BRICK_H
#define BRICK_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "entity_type.h"
#include "resource.h"
#include "animation_handler.h"

struct Entities;

typedef struct Brick
{
    EntityType type;
    b2WorldId world_id;
    b2BodyId body;
    bool active;
    bool is_destroying;

    struct Entities *entities;
    Subtexture *subtexture;
    AnimationHandler *animation_handler;
    b2Vec2 size;
    float health;
    float max_health;
    int brick_type;

    void (*update)(struct Brick *brick, float delta_time);
    void (*render)(struct Brick *brick);
    void (*clean_up)(struct Brick *brick);
    void (*reset)(struct Brick *brick, b2Vec2 position, int brick_type);
    void (*disable)(struct Brick *brick);
} Brick;

Brick *create_brick(struct Entities *Entities, b2WorldId world_id, b2Vec2 position, int brick_type);

#endif
