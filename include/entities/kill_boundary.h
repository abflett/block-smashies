#ifndef KILL_BOUNDARY_H
#define KILL_BOUNDARY_H

#include "box2d/box2d.h"
#include "entity_type.h"

typedef struct KillBoundary
{
    EntityType type;
    b2BodyId body;

    void (*clean_up)(struct KillBoundary *kill_boundary);
} KillBoundary;

KillBoundary *create_kill_boundary(const b2WorldId world_id);

#endif