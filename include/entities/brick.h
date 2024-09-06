#ifndef BRICK_H
#define BRICK_H

#include <stdbool.h>
#include "raylib.h"

struct Entities;

typedef struct Brick
{
    Texture2D texture;
    Vector2 position;
    int health;
    bool active;
    void (*update)(struct Brick *brick, struct Entities *entities, float delta_time);
    void (*render)(struct Brick *brick);
    Rectangle (*get_hitbox)(struct Brick *brick);
} Brick;

Brick create_brick(Vector2 initial_position);

#endif
