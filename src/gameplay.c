#include "gameplay.h"
#include "raylib.h"
#include "scene_manager.h"
#include "main_menu.h"
#include <math.h>

extern SceneManager scene_manager;

// Static variables, scoped only to this file
static float paddle_x;
static int paddle_y;
static int paddle_size_x;
static int paddle_size_y;
static float speed;
static float acceleration;
static float max_speed;
static float friction;

// Ball properties
static Vector2 ball_position;
static Vector2 ball_velocity;
static float ball_radius;

Scene gameplay_scene = {
    .init = gameplay_init,
    .update = gameplay_update,
    .render = gameplay_render,
    .cleanup = gameplay_cleanup,
};

void gameplay_init(void)
{
    // Resetting paddle variables
    paddle_x = 150.0f;
    paddle_y = 170;
    paddle_size_x = 20;
    paddle_size_y = 5;
    speed = 0.0f;
    acceleration = 500.0f;
    max_speed = 300.0f;
    friction = 0.95f;

    // Initialize ball properties
    ball_position = (Vector2){160.0f, 90.0f};   // Start in the middle of the screen
    ball_velocity = (Vector2){100.0f, -100.0f}; // Initial velocity
    ball_radius = 1.5f;
}

void gameplay_update(float delta_time)
{
    // Update paddle speed based on input
    if (IsKeyDown(KEY_A))
    {
        speed -= acceleration * delta_time;
        if (speed < -max_speed)
            speed = -max_speed;
    }
    if (IsKeyDown(KEY_D))
    {
        speed += acceleration * delta_time;
        if (speed > max_speed)
            speed = max_speed;
    }

    // Apply friction when no keys are pressed
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
    {
        speed *= friction;
        if (fabs(speed) < 0.01f)
            speed = 0.0f; // Stop movement if speed is very low
    }

    // Update paddle position
    paddle_x += speed * delta_time;

    // Keep paddle within screen bounds
    if (paddle_x < 0)
        paddle_x = 0;
    if (paddle_x + paddle_size_x > 320)
        paddle_x = 320.0f - paddle_size_x;

    // Update ball position
    ball_position.x += ball_velocity.x * delta_time;
    ball_position.y += ball_velocity.y * delta_time;

    // Ball collision with screen bounds
    if (ball_position.x - ball_radius <= 0 || ball_position.x + ball_radius >= 320)
    {
        ball_velocity.x *= -1; // Reverse horizontal direction
    }
    if (ball_position.y - ball_radius <= 0)
    {
        ball_velocity.y *= -1; // Reverse vertical direction
    }

    // Ball collision with paddle
    if (ball_position.y + ball_radius >= paddle_y &&
        ball_position.x >= paddle_x &&
        ball_position.x <= paddle_x + paddle_size_x)
    {
        ball_velocity.y *= -1;                    // Reverse vertical direction
        ball_position.y = paddle_y - ball_radius; // Prevent the ball from sticking to the paddle

        // Adjust the ball's x-velocity based on the paddle's speed
        ball_velocity.x += speed * 0.5f; // Scale the influence of the paddle's speed

        // Ensure the ball's x-velocity doesn't exceed a certain maximum
        if (fabs(ball_velocity.x) > max_speed)
        {
            ball_velocity.x = (ball_velocity.x > 0) ? max_speed : -max_speed;
        }
    }

    // Ball falls below the paddle (you can add life loss or game over logic here)
    if (ball_position.y + ball_radius > 180)
    {
        gameplay_init(); // Reset game if the ball falls below the screen
    }

    // Switch to the main menu scene if ENTER is pressed
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.next_scene = &main_menu_scene;
    }
}

void gameplay_render(void)
{
    ClearBackground(BLACK); // Clear the screen with a black background
    DrawText("Gameplay", 5, 5, 8, WHITE);
    DrawText("Press ENTER to Main Menu", 5, 16, 8, WHITE);
    DrawRectangle((int)paddle_x, paddle_y, paddle_size_x, paddle_size_y, RED);
    DrawCircleV(ball_position, ball_radius, GREEN); // Draw the ball
}

void gameplay_cleanup(void)
{
    // Cleanup resources used by the gameplay scene
}
