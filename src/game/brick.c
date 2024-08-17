#include "brick.h"
#include "entities.h"
#include "raylib.h"

// Function to update the brick
static void update_brick(Brick *brick, struct Entities *entities, float delta_time)
{
    // Placeholder for brick-specific update logic
    // For example, you could reduce health based on some condition or trigger effects
    if (brick->health <= 0)
    {
        brick->active = false; // Deactivate the brick when health is depleted
    }
}

// Function to render the brick
static void render_brick(Brick *brick)
{
    if (brick->active)
    {
        //DrawTexture(brick->texture, (int)brick->position.x, (int)brick->position.y, WHITE);
        DrawRectangle(brick->position.x, brick->position.y, 30, 5, RED);
    }
}

// Function to get the hitbox of the brick
static Rectangle get_brick_hitbox(Brick *brick)
{
    return (Rectangle){brick->position.x, brick->position.y, 30.0f, 5.0f};
}

// Function to create and initialize a brick
Brick create_brick(Vector2 initial_position)
{
    Brick brick;
    //brick.texture = LoadTexture("brick.png");  // Load the brick texture (make sure to manage texture unloading later)
    brick.position = initial_position;
    brick.health = 1;  // Initial health, you can adjust this as needed
    brick.active = true;
    brick.update = update_brick;
    brick.render = render_brick;
    brick.get_hitbox = get_brick_hitbox;

    return brick;
}
