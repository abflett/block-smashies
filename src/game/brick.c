#include "brick.h"
#include "entities.h"
#include "raylib.h"

static void update_brick(Brick *brick, struct Entities *entities, float delta_time)
{
    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *ball = &kv_A(entities->balls, i);
        if (ball->active)
        {
            Rectangle hitbox = brick->get_hitbox(brick);
            bool collision = CheckCollisionCircleRec(ball->position, ball->radius, hitbox);
            if (collision)
            {
                brick->health--;

                // Check where the ball hit the brick
                float ball_center_x = ball->position.x;
                float ball_center_y = ball->position.y;

                // Check if the ball hit the top or bottom of the brick
                if (ball_center_y < hitbox.y || ball_center_y > hitbox.y + hitbox.height)
                {
                    ball->velocity.y *= -1; // Reverse the ball's vertical direction
                }
                // Check if the ball hit the left or right side of the brick
                else if (ball_center_x < hitbox.x || ball_center_x > hitbox.x + hitbox.width)
                {
                    ball->velocity.x *= -1; // Reverse the ball's horizontal direction
                }

                // Optional: Adjust the ball's position slightly to prevent "sticking"
                if (ball_center_y < hitbox.y)
                {
                    ball->position.y = hitbox.y - ball->radius;
                }
                else if (ball_center_y > hitbox.y + hitbox.height)
                {
                    ball->position.y = hitbox.y + hitbox.height + ball->radius;
                }
                else if (ball_center_x < hitbox.x)
                {
                    ball->position.x = hitbox.x - ball->radius;
                }
                else if (ball_center_x > hitbox.x + hitbox.width)
                {
                    ball->position.x = hitbox.x + hitbox.width + ball->radius;
                }

                if (brick->health <= 0)
                {
                    brick->active = false;
                }
            }
        }
    }
}

static void render_brick(Brick *brick)
{
    if (brick->active)
    {
        // DrawTexture(brick->texture, (int)brick->position.x, (int)brick->position.y, WHITE);
        DrawRectangle(brick->position.x, brick->position.y, 20, 5, RED);
    }
}

static Rectangle get_brick_hitbox(Brick *brick)
{
    return (Rectangle){brick->position.x, brick->position.y, 20.0f, 5.0f};
}

Brick create_brick(Vector2 initial_position)
{
    Brick brick;
    // brick.texture = LoadTexture("brick.png");  // Load the brick texture (make sure to manage texture unloading later)
    brick.position = initial_position;
    brick.health = 1;
    brick.active = true;
    brick.update = update_brick;
    brick.render = render_brick;
    brick.get_hitbox = get_brick_hitbox;

    return brick;
}
