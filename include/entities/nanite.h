#ifndef NANITE_H
#define NANITE_H

#include <stdbool.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "entity_type.h"

struct Entities;

typedef struct Nanite
{
    EntityType type;
    b2BodyId body;
    bool active;

    Texture2D *texture;
    b2Vec2 size;
    int currency;

    void (*update)(struct Nanite *nanite, float delta_time);
    void (*render)(struct Nanite *nanite);
    void (*clean_up)(struct Nanite *nanite);
    void (*reset)(struct Nanite *nanite, b2Vec2 position, int currency);
    void (*disable)(struct Nanite *nanite);
} Nanite;

Nanite *create_nanite(b2WorldId world_id, b2Vec2 position, int currency);

#endif