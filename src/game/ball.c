#include "ball.h"
#include "resource_manager.h"
#include "game_settings.h"

// Function to update the ball's position and handle collisions
void update_ball(Ball *ball, float delta_time)
{
    // Update ball position based on its velocity
    ball->position.x += ball->velocity.x * ball->speed_multiplier * delta_time;
    ball->position.y += ball->velocity.y * ball->speed_multiplier * delta_time;

    // Ball collision with screen bounds
    if (ball->position.x - ball->radius <= 0 || ball->position.x + ball->radius >= game_settings.target_width)
    {
        ball->velocity.x *= -1; // Reverse horizontal direction
    }
    if (ball->position.y - ball->radius <= 0)
    {
        ball->velocity.y *= -1; // Reverse vertical direction
    }
}

// Function to reset the ball's position and velocity
void reset_ball(Ball *ball, Vector2 initial_position)
{
    ball->position = initial_position;
    ball->velocity = (Vector2){100.0f, -100.0f}; // Reset to default velocity
    ball->speed_multiplier = 1.0f;               // Reset speed multiplier
}

// Function to render the ball
void render_ball(Ball *ball)
{
    DrawTexture(ball->texture, (int)(ball->position.x - ball->radius), (int)(ball->position.y - ball->radius), WHITE);
}

// Function to create and initialize a ball
Ball create_ball(Vector2 initial_position)
{
    Ball ball;
    ball.texture = resource_manager.get_texture("ball")->texture;
    ball.radius = ball.texture.width / 2.0f; // Assuming the ball is circular
    ball.position = initial_position;
    ball.velocity = (Vector2){100.0f, -100.0f}; // Example initial velocity
    ball.speed_multiplier = 1.0f;
    ball.update = update_ball;
    ball.reset = reset_ball;
    ball.render = render_ball;

    return ball;
}
