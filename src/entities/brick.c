#include "raylib.h"
#include "brick.h"
#include "entities.h"
#include "resource_manager.h"

static void update_brick(Brick *brick, struct Entities *entities, float delta_time)
{
}

static void render_brick(Brick *brick)
{
    if (brick->active)
    {
        DrawTexture(brick->texture, (int)brick->position.x, (int)brick->position.y, WHITE);
    }
}

static Rectangle get_brick_hitbox(Brick *brick)
{
    return (Rectangle){brick->position.x, brick->position.y, (float)brick->texture.width, (float)brick->texture.height};
}

Brick create_brick(Vector2 initial_position)
{
    Brick brick;
    brick.texture = resource_manager.get_texture("greenbrick")->texture; // Load the brick texture (make sure to manage texture unloading later)
    brick.position = initial_position;
    brick.health = 1;
    brick.active = true;
    brick.update = update_brick;
    brick.render = render_brick;
    brick.get_hitbox = get_brick_hitbox;

    return brick;
}
