#include "gameplay.h"
#include "raylib.h"
#include "scene_manager.h"
#include "main_menu.h"
#include <math.h>

extern SceneManager scene_manager;

Scene gameplay_scene = {
    .init = gameplay_init,
    .update = gameplay_update,
    .render = gameplay_render,
    .cleanup = gameplay_cleanup,
};

float paddle_x = 150.0f;
int paddle_y = 170;
int paddle_size_x = 20;
int paddle_size_y = 5;
float speed = 0.0f;          // Current speed
float acceleration = 500.0f; // How fast the paddle accelerates
float max_speed = 300.0f;    // Maximum speed
float friction = 0.95f;      // Friction factor to gradually slow down

void gameplay_init(void)
{
    // Initialize the gameplay scene
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
}

void gameplay_cleanup(void)
{
    // Cleanup resources used by the gameplay scene
}
