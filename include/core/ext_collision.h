#ifndef EXT_COLLISION_H
#define EXT_COLLISION_H

#include "raylib.h"
#include "stdbool.h"

typedef enum CollisionSide
{
    SIDE_NONE,
    SIDE_LEFT,
    SIDE_RIGHT,
    SIDE_TOP,
    SIDE_BOTTOM
} CollisionSide;

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

typedef struct CollisionResult
{
    bool collided;        // has collided
    Vector2 point;        // point of collision
    float remaining_line; // remaining line after subracting from point of collision
    CollisionSide side;   // which side was collided with
} CollisionResult;

CollisionResult check_collision_thick_line_rect(Vector2 line_start, Vector2 line_end, float radius, Rectangle rect);

#endif