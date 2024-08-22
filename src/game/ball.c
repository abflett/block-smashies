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
        Paddle *paddle = kv_A(entities->paddles, i);
        if (paddle->active)
        {
            CollisionResult collision_result = check_collision_thick_line_rect(ball->position,
                                                                               ball_end,
                                                                               ball->radius,
                                                                               paddle->get_hitbox(paddle));

            if (collision_result.collided)
            {
                if (collision_result.side == SIDE_TOP || collision_result.side == SIDE_BOTTOM)
                {
                    ball->velocity.y *= -1;
                }
                else if (collision_result.side == SIDE_LEFT || collision_result.side == SIDE_RIGHT)
                {
                    ball->velocity.x *= -1;
                }

                if (fabs(paddle->speed) > 0.0f)
                    ball->velocity.x += paddle->speed * 0.5f; // Scale the influence of the paddle's speed

                if (fabs(ball->velocity.x) > *ball->max_speed)
                {
                    ball->velocity.x = (ball->velocity.x > 0) ? *ball->max_speed : -(*ball->max_speed);
                }

                Vector2 remaining_movement = Vector2Scale(
                    Vector2Normalize(ball->velocity),
                    collision_result.remaining_line);

                ball->position = Vector2Add(collision_result.point, remaining_movement);

                entities->game_status.score += 10;
                not_collided = false;
            }
        }
    }

    return not_collided;
}

static bool handle_brick_collisions(Ball *ball, Entities *entities, float delta_time)
{
    bool not_collided = true;
    Vector2 ball_end = Vector2Add(ball->position,
                                  (Vector2){ball->velocity.x * (*ball->speed_multiplier) * delta_time,
                                            ball->velocity.y * (*ball->speed_multiplier) * delta_time});

    Brick *collided_bricks[10]; // Allow up to 10 simultaneous collisions
    CollisionResult collision_results[10];
    int collision_count = 0;

    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = kv_A(entities->bricks, i);
        if (brick->active)
        {
            CollisionResult collision_result = check_collision_thick_line_rect(
                ball->position,
                ball_end,
                ball->radius,
                brick->get_hitbox(brick));

            if (collision_result.collided && collision_count < 10)
            {
                collided_bricks[collision_count] = brick;
                collision_results[collision_count] = collision_result;
                collision_count++;
            }
        }
    }

    if (collision_count == 0)
        return true; // No collisions detected

    int closest = 0;
    float closest_line = 320.0f; // Use a value representing the max distance in your game
    for (int i = 0; i < collision_count; i++)
    {
        float temp_line = collision_results[i].remaining_line;
        if (temp_line < closest_line)
        {
            closest_line = temp_line;
            closest = i;
        }
    }

    // Handle the closest collision (e.g., reflect ball, deactivate brick)
    Brick *closest_brick = collided_bricks[closest];
    CollisionResult closest_collision = collision_results[closest];

    closest_brick->health -= *ball->power;
    if (closest_brick->health <= 0)
    {
        closest_brick->active = false;
    }

    // move ball by the remaining_line magnitude and velocity unit based on the side of collision
    // closest_collision.remaining_line - remaining length/magnitude
    // ball->velocity - need to convert to a velocity unit for the new position
    switch (closest_collision.side)
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

    Vector2 velocity_unit = Vector2Normalize(ball->velocity);
    ball->position = Vector2Add(closest_collision.point, Vector2Scale(velocity_unit, closest_collision.remaining_line));
    return false;
}

static void handle_wall_collisions(Ball *ball)
{
    if (ball->position.x - ball->radius <= game_settings.play_area.x)
    {
        ball->velocity.x *= -1;
        ball->position.x = game_settings.play_area.x + ball->radius;
    }

    if (ball->position.x + ball->radius >= game_settings.play_area.width + game_settings.play_area.x)
    {
        ball->velocity.x *= -1;
        ball->position.x = game_settings.play_area.width + game_settings.play_area.x - ball->radius;
    }

    if (ball->position.y - ball->radius <= game_settings.play_area.y)
    {
        ball->velocity.y *= -1;
        ball->position.y = game_settings.play_area.y + ball->radius;
    }
}

static void handle_out_of_bounds(Ball *ball, Entities *entities)
{
    if (ball->position.y > game_settings.play_area.height + game_settings.play_area.y)
    {
        ball->active = false;
        int active_balls = 0;
        for (int j = 0; j < kv_size(entities->balls); j++)
        {
            Ball *other_ball = kv_A(entities->balls, j);
            if (other_ball->active)
            {
                active_balls++;
            }
        }

        if (active_balls == 0)
        {
            entities->game_status.lives--;
            ball->reset(ball, (Vector2){160.0f, 90.0f});
            Paddle *first_paddle = kv_A(entities->paddles, 0);
            first_paddle->reset(first_paddle);
        }
    }
}

static void update_ball(Ball *ball, Entities *entities, float delta_time)
{

    bool not_collided = true;

    if (not_collided)
        not_collided = handle_ball_paddle_collision(ball, entities, delta_time);

    if (not_collided)
        not_collided = handle_brick_collisions(ball, entities, delta_time);

    if (not_collided)
    {
        ball->position.x += ball->velocity.x * *ball->speed_multiplier * delta_time;
        ball->position.y += ball->velocity.y * *ball->speed_multiplier * delta_time;
    }

    handle_wall_collisions(ball);
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