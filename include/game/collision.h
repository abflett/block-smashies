#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"

// Enumeration for different collision types
typedef enum
{
    COLLISION_NONE,
    COLLISION_WALL,
    COLLISION_CEILING,
    COLLISION_PADDLE,
    COLLISION_BRICK,
    COLLISION_ENEMY,
    COLLISION_COLLECTABLE,
    COLLISION_MISSILE,
    COLLISION_BOTTOM
} CollisionType;

// Structure to represent a collision event
typedef struct
{
    CollisionType type;
    Rectangle hitbox; // The hitbox of the object involved in the collision
    Vector2 point;    // The point of collision
} CollisionEvent;

// Function prototypes
CollisionEvent check_collision_with_walls(Rectangle object);
CollisionEvent check_collision_with_paddle(Rectangle object, Rectangle paddle);
CollisionEvent check_collision_with_bottom(Rectangle object);

#endif
