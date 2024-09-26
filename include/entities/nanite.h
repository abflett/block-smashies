#ifndef NANITE_H
#define NANITE_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "entity_type.h"
#include "resource_manager.h"

struct Entities;

typedef struct Nanite
{
    EntityType type;
    int nanite_type;
    b2BodyId body;
    bool active;

    Subtexture *subtexture;
    b2Vec2 size;
    float current_rotation;
    float rotation_speed;
    bool large_nanite;
    b2ShapeId shape_id;
    float currency;

    void (*update)(struct Nanite *nanite, float delta_time);
    void (*render)(struct Nanite *nanite);
    void (*clean_up)(struct Nanite *nanite);
    void (*reset)(struct Nanite *nanite, b2Vec2 position, float currency, int nanite_type);
    void (*disable)(struct Nanite *nanite);
} Nanite;

Nanite *create_nanite(b2WorldId world_id, b2Vec2 position, float currency, int nanite_type);

#endif