#ifndef KILL_BOUNDARY_H
#define KILL_BOUNDARY_H

#include "box2d/box2d.h"

typedef struct KillBoundary
{
    b2BodyId body;

    void (*clean_up)(struct KillBoundary *kill_boundary);
} KillBoundary;

KillBoundary create_kill_boundary(b2WorldId world_id);

#endif