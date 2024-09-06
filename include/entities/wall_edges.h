#ifndef WALL_EDGES_H
#define WALL_EDGES_H

#include "box2d/box2d.h"

typedef struct WallEdges
{
    b2BodyId body; // Single body for all edges

    void (*clean_up)(struct WallEdges *walls); // Clean-up function pointer
} WallEdges;

WallEdges create_wall_edges(b2WorldId world_id);

#endif