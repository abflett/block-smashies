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

static void handle_ball_paddle_collision(Ball *ball, Paddle *paddle, Entities *entities, float delta_time)
{
    CollisionResult collision_result = check_collision_thick_line_rect(
        ball->position,
        Vector2Add(
            ball->position,
            (Vector2){
                ball->velocity.x * *ball->speed_multiplier * delta_time,
                ball->velocity.y * *ball->speed_multiplier * delta_time}),
        ball->radius,
        paddle->get_hitbox(paddle));

    if (collision_result.collided)
    {
        TraceLog(LOG_INFO, "Gonna collide with paddle, remaining line: %f", collision_result.remaining_line);

        ball->velocity.y *= -1;
        ball->position.y = paddle->position.y - ball->radius;

        // Adjust the ball's x-velocity based on the paddle's speed
        if (paddle->speed > 0.0f)
            ball->velocity.x += paddle->speed * 0.5f; // Scale the influence of the paddle's speed

        // Ensure the ball's x-velocity doesn't exceed a certain maximum
        if (fabs(ball->velocity.x) > *paddle->max_speed)
        {
            ball->velocity.x = (ball->velocity.x > 0) ? *paddle->max_speed : -(*paddle->max_speed);
        }

        // Increase score when the ball hits the paddle
        entities->game_status.score += 10;
    }
}

static void handle_wall_collisions(Ball *ball)
{
    if (ball->position.x - ball->radius <= game_settings.play_area.x || ball->position.x + ball->radius >= game_settings.play_area.width + game_settings.play_area.x)
    {
        ball->velocity.x *= -1;
    }

    if (ball->position.y - ball->radius <= game_settings.play_area.y)
    {
        ball->velocity.y *= -1;
    }
}

static void update_ball(Ball *ball, Entities *entities, float delta_time)
{
    ball->position.x += ball->velocity.x * *ball->speed_multiplier * delta_time;
    ball->position.y += ball->velocity.y * *ball->speed_multiplier * delta_time;

    // Handle wall collisions
    handle_wall_collisions(ball);

    // Handle paddle collisions
    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (paddle->active)
        {
            handle_ball_paddle_collision(ball, paddle, entities, delta_time);
        }
    }

    // Check if the ball is out of bounds
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

        // Additional logic if last active ball
        if (active_balls == 0)
        {
            entities->game_status.lives--;
            ball->reset(ball, (Vector2){160.0f, 90.0f});
            Paddle *first_paddle = &kv_A(entities->paddles, 0);
            first_paddle->reset(first_paddle);
        }
    }
}

static void reset_ball(Ball *ball, Vector2 initial_position)
{
    ball->position = initial_position;
    ball->velocity = (Vector2){50.0f, -50.0f};
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
    ball.active = true;
    ball.update = update_ball;
    ball.reset = reset_ball;
    ball.render = render_ball;

    return ball;
}