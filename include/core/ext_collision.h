#ifndef EXT_COLLISION_H
#define EXT_COLLISION_H

#include "raylib.h"
#include "stdbool.h"

typedef struct Edge
{
    Vector2 start;
    Vector2 end;
} Edge;

typedef struct Edges
{
    Edge left;
    Edge right;
    Edge top;
    Edge bottom;
} Edges;

typedef struct PointLine
{
    bool collision; // has collided
    Vector2 point;  // point of collision
    float line;     // remaining line after subracting from point of collision
} PointLine;

PointLine check_collision_thick_line_rect(Vector2 line_start, Vector2 line_end, float radius, Rectangle rect);

#endif