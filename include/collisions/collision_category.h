#ifndef COLLISION_CATEGORY_H
#define COLLISION_CATEGORY_H

// Define the collision category bits
typedef enum CollisionCategory
{
    CATEGORY_NONE = 0x0000,          // No category
    CATEGORY_BALL = 0x0002,          // Ball category
    CATEGORY_BRICK = 0x0004,         // Brick category
    CATEGORY_PADDLE = 0x0008,        // Paddle category
    CATEGORY_NANITE = 0x0010,        // Nanite category
    CATEGORY_WALL = 0x0020,          // Wall category
    CATEGORY_KILL_BOUNDARY = 0x0040, // Kill boundary category
    CATEGORY_ALL = 0xFFFF            // Collide with all categories
} CollisionCategory;

#define BALL_COLLIDE_WITH (CATEGORY_ALL & ~CATEGORY_BALL)              // Ball collides with everything except other balls
#define PADDLE_COLLIDE_WITH (CATEGORY_ALL & ~CATEGORY_PADDLE)          // Paddle collides with everything except paddles
#define NANITE_COLLIDE_WITH (CATEGORY_PADDLE | CATEGORY_KILL_BOUNDARY) // Nanites only collide with paddles
#define KILL_BOUNDARY_COLLIDE_WITH (CATEGORY_ALL)                      // Kill boundary collides with everything

#endif