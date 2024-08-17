#include "ball.h"
#include "entities.h"
#include "resource_manager.h"
#include "game_settings.h"


static void update_ball(Ball *ball, Entities *entities, float delta_time)
{
    ball->position.x += ball->velocity.x * ball->speed_multiplier * delta_time;
    ball->position.y += ball->velocity.y * ball->speed_multiplier * delta_time;

    if (ball->position.x - ball->radius <= 0 || ball->position.x + ball->radius >= game_settings.target_width)
    {
        ball->velocity.x *= -1;
    }
    if (ball->position.y - ball->radius <= 0)
    {
        ball->velocity.y *= -1;
    }

    for (int i = 0; i < kv_size(entities->paddles); i++) {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (paddle->active) {
            //TraceLog(LOG_INFO, "Paddle %d X Position: %f", i, paddle->position.x);
        }
    }
}

static void reset_ball(Ball *ball, Vector2 initial_position)
{
    ball->position = initial_position;
    ball->velocity = (Vector2){100.0f, -100.0f};
    ball->speed_multiplier = 1.0f;
}

static void render_ball(Ball *ball)
{
    // draw larger ball and resize down for subpixel drawing effect
    DrawTextureEx(ball->texture, ball->position, 0.0f, 0.5f, WHITE);
}

Ball create_ball(Vector2 initial_position)
{
    Ball ball;
    ball.texture = resource_manager.get_texture("ball")->texture;
    ball.radius = ball.texture.width / 2.0f; // Todo: check as the texture is scaled down.
    ball.position = initial_position;
    ball.velocity = (Vector2){100.0f, -100.0f};
    ball.speed_multiplier = 1.0f;
    ball.active = true;
    ball.update = update_ball;
    ball.reset = reset_ball;
    ball.render = render_ball;

    return ball;
}
