#include <stdbool.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"
#include "ball.h"
#include "entities.h"
#include "resource_manager.h"
#include "game_settings.h"
#include "player.h"
#include "ext_collision.h"

static bool handle_ball_paddle_collision(Ball *ball, Entities *entities, float delta_time)
{
    bool not_collided = true;
    Vector2 ball_end = Vector2Add(ball->position,
                                  (Vector2){ball->velocity.x * (*ball->speed_multiplier) * delta_time,
                                            ball->velocity.y * (*ball->speed_multiplier) * delta_time});

    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (paddle->active)
        {
            CollisionResult collision_result = check_collision_thick_line_rect(ball->position,
                                                                               ball_end,
                                                                               ball->radius,
                                                                               paddle->get_hitbox(paddle));

            if (collision_result.collided)
            {
                Vector2 ball_step = Vector2Subtract(ball->position, ball_end);
                Vector2 ball_start_collision_point = Vector2Subtract(ball->position, collision_result.point);
                TraceLog(LOG_INFO, "Ball step x%f, y%f", ball_step.x, ball_step.y);
                TraceLog(LOG_INFO, "coll step x%f, y%f", ball_start_collision_point.x, ball_start_collision_point.y);

                TraceLog(LOG_INFO, "ball collided with paddle, remaining line: %f, collision point.x: %f, point.y %f, side %d", collision_result.remaining_line, collision_result.point.x, collision_result.point.y, (int)collision_result.side);

                // Adjust the ball's velocity based on the collision side
                // If the collision side is the top, reverse the Y velocity
                if (collision_result.side == SIDE_TOP)
                {
                    ball->velocity.y *= -1;
                    ball->position.y = paddle->position.y - ball->radius - 0.1f; // Set the position to just above the paddle
                }
                else if (collision_result.side == SIDE_LEFT || collision_result.side == SIDE_RIGHT)
                {
                    ball->velocity.x *= -1;
                    // Optional: adjust the Y position slightly to avoid sinking into the paddle
                    // ball->position.y = collision_result.point.y - ball->radius;
                }

                // Adjust the ball's x-velocity based on the paddle's speed
                if (paddle->speed > 0.0f)
                    ball->velocity.x += paddle->speed * 0.5f; // Scale the influence of the paddle's speed

                // Ensure the ball's x-velocity doesn't exceed a certain maximum
                if (fabs(ball->velocity.x) > *ball->max_speed)
                {
                    ball->velocity.x = (ball->velocity.x > 0) ? *ball->max_speed : -(*ball->max_speed);
                }

                // Adjust the ball's position based on the collision point and remaining movement
                Vector2 remaining_movement = Vector2Scale(
                    Vector2Normalize(ball->velocity),
                    collision_result.remaining_line);

                ball->position = Vector2Add(collision_result.point, remaining_movement);

                // Increase score when the ball hits the paddle
                entities->game_status.score += 10;
                not_collided = false;
            }
        }
    }

    return not_collided;
}

static void handle_brick_collisions(Ball *ball, Entities *entities, float delta_time)
{
    Vector2 ball_end = Vector2Add(ball->position,
                                  (Vector2){ball->velocity.x * (*ball->speed_multiplier) * delta_time,
                                            ball->velocity.y * (*ball->speed_multiplier) * delta_time});

    Ball collided_balls[3];
    CollisionResult collision_results[3];

    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = &kv_A(entities->bricks, i);
        if (brick->active)
        {
            CollisionResult collision_result = check_collision_thick_line_rect(
                ball->position,
                ball_end,
                ball->radius,
                brick->get_hitbox(brick));

            if (collision_result.collided)
            {
                switch (collision_result.side)
                {
                case SIDE_LEFT:
                case SIDE_RIGHT:
                    ball->velocity.x *= -1;
                    break;

                case SIDE_TOP:
                case SIDE_BOTTOM:
                    ball->velocity.y *= -1;
                    break;

                default:
                    break;
                }

                brick->health -= *ball->power;
                if (brick->health <= 0)
                {
                    brick->active = false;
                }
            }
        }
    }
}

static void handle_wall_collisions(Ball *ball)
{
    // Todo: move ball
    if (ball->position.x - ball->radius <= game_settings.play_area.x || ball->position.x + ball->radius >= game_settings.play_area.width + game_settings.play_area.x)
    {
        ball->velocity.x *= -1;
    }

    if (ball->position.y - ball->radius <= game_settings.play_area.y)
    {
        ball->velocity.y *= -1;
    }
}

static void handle_out_of_bounds(Ball *ball, Entities *entities)
{
    if (ball->position.y > game_settings.play_area.height + game_settings.play_area.y)
    {
        ball->active = false;

        // Count the remaining active balls
        int active_balls = 0;
        for (int j = 0; j < kv_size(entities->balls); j++)
        {
            Ball *other_ball = &kv_A(entities->balls, j);
            if (other_ball->active)
            {
                active_balls++;
            }
        }

        // Additional logic if last active ball Todo: move death logic or reset out of this check.
        if (active_balls == 0)
        {
            entities->game_status.lives--;
            ball->reset(ball, (Vector2){160.0f, 90.0f});
            Paddle *first_paddle = &kv_A(entities->paddles, 0);
            first_paddle->reset(first_paddle);
        }
    }
}

static void update_ball(Ball *ball, Entities *entities, float delta_time)
{

    bool not_collided = true;

    // handle paddle collisions

    if (not_collided)
        not_collided = handle_ball_paddle_collision(ball, entities, delta_time);

    // handle brick collisions
    if (not_collided)
    {
        ball->position.x += ball->velocity.x * *ball->speed_multiplier * delta_time;
        ball->position.y += ball->velocity.y * *ball->speed_multiplier * delta_time;
    }

    if (not_collided)
        handle_brick_collisions(ball, entities, delta_time);

    // Handle wall collisions
    handle_wall_collisions(ball);

    // Check if the ball is out of bounds
    handle_out_of_bounds(ball, entities);
}

static void reset_ball(Ball *ball, Vector2 initial_position)
{
    ball->position = initial_position;
    ball->velocity = (Vector2){*ball->max_speed, -(*ball->max_speed)};
    *ball->speed_multiplier = 1.0f;
}

static void render_ball(Ball *ball)
{
    // draw larger ball and resize down for subpixel drawing effect
    DrawTextureEx(ball->texture, (Vector2){ball->position.x - ball->radius, ball->position.y - ball->radius}, 0.0f, 0.5f, WHITE);
}

Ball create_ball(Player *player, Vector2 position, Vector2 velocity)
{
    Ball ball;
    ball.texture = resource_manager.get_texture("ball")->texture;
    ball.radius = ball.texture.width / 4.0f; // Todo: check as the texture is scaled down.
    ball.position = position;
    ball.velocity = velocity;
    ball.speed_multiplier = &player->ball.speed_multiplier;
    ball.power = &player->ball.power;
    ball.phase_nova = &player->perks.phase_shift;
    ball.super_nova = &player->perks.super_nova;
    ball.max_speed = &player->ball.max_speed;
    ball.active = true;
    ball.update = update_ball;
    ball.reset = reset_ball;
    ball.render = render_ball;

    return ball;
}