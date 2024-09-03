#ifndef WALL_EDGES_H
#define WALL_EDGES_H

#include "box2d/box2d.h"
#include "raylib.h"

typedef struct WallEdges
{
    b2BodyId left_wall;
    b2BodyId right_wall;
    b2BodyId top_wall;
    Rectangle bounds; // Defines the play area for easy reference

    void (*clean_up)(struct WallEdges *walls);
} WallEdges;

WallEdges create_wall_edges(b2WorldId world_id, Rectangle play_area);

#endif
