#include "collision.h"
#include "game_settings.h"

// Check collision with walls
CollisionEvent check_collision_with_walls(Rectangle object)
{
    CollisionEvent event = {.type = COLLISION_NONE};

    // Check collision with left wall
    if (object.x <= 0)
    {
        event.type = COLLISION_WALL;
        event.point = (Vector2){object.x, object.y};
        event.hitbox = (Rectangle){0.0f, object.y, 0.0f, object.height};
    }
    // Check collision with right wall
    else if (object.x + object.width >= (float)game_settings.target_width)
    {
        event.type = COLLISION_WALL;
        event.point = (Vector2){object.x + object.width, object.y};
        event.hitbox = (Rectangle){(float)game_settings.target_width, object.y, 0, object.height};
    }
    // Check collision with ceiling
    else if (object.y <= 0)
    {
        event.type = COLLISION_CEILING; // COLLISION_CEILING
        event.point = (Vector2){object.x, object.y};
        event.hitbox = (Rectangle){object.x, 0, object.width, 0};
    }

    return event;
}

// Check if object falls below the screen
CollisionEvent check_collision_with_bottom(Rectangle object)
{
    CollisionEvent event = {.type = COLLISION_NONE};

    if (object.y >= game_settings.target_height)
    {
        event.type = COLLISION_BOTTOM;
        event.point = (Vector2){object.x, object.y + object.height};
    }

    return event;
}

// Check collision with paddle
CollisionEvent check_collision_with_paddle(Rectangle object, Rectangle paddle)
{
    CollisionEvent event = {.type = COLLISION_NONE};

    if (CheckCollisionRecs(object, paddle))
    {
        event.type = COLLISION_PADDLE;
        event.point = (Vector2){object.x, object.y};
        event.hitbox = paddle;
    }

    return event;
}
