#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "playing_state.h"
#include "play_state_manager.h"
#include "playing_menu_state.h"
#include "high_score.h"

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

// Game status variables
static int player_lives;
static int player_score;
static float game_time;

HighScore high_scores[10];
int count = 0;

Texture2D ball_texture;

PlayState playing_state = {
    .init = playing_state_init,
    .update = playing_state_update,
    .render = playing_state_render,
    .cleanup = playing_state_cleanup,
};

void playing_state_init(void)
{
    ball_texture = LoadTexture("ball.png");
    // Load high scores from file
    load_high_scores("high_scores.json", high_scores, &count);

    // Add a new high score
    add_high_score(high_scores, &count, "Adam", 1000000);

    // Save high scores to file
    save_high_scores("high_scores.json", high_scores, count);

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

    // Initialize game status variables
    player_lives = 3;
    player_score = 0;
    game_time = 0.0f;
}

void playing_state_update(float delta_time)
{
    // Update game time
    game_time += delta_time;

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

        // Increase score when the ball hits the paddle
        player_score += 10;
    }

    // Ball falls below the paddle (you can add life loss or game over logic here)
    if (ball_position.y + ball_radius > 180)
    {
        player_lives--;
        if (player_lives <= 0)
        {
            // Game over logic here (for now, just reset the game)
            playing_state_init();
        }
        else
        {
            // Reset ball position and velocity
            ball_position = (Vector2){160.0f, 90.0f};
            ball_velocity = (Vector2){100.0f, -100.0f};
        }
    }

    // // Switch to the main menu scene if ENTER is pressed
    if (IsKeyPressed(KEY_ESCAPE))
    {
        play_state_manager.next_state = &playing_menu_state;
    }
}

void playing_state_render(void)
{
    char score_text[20];
    snprintf(score_text, sizeof(score_text), "%d", player_score);
    DrawText(score_text, 5, 5, 8, LIGHTGRAY);
    // Draw game information

    char time_text[20];
    int minutes = (int)(game_time / 60);
    float seconds = game_time - minutes * 60;
    snprintf(time_text, sizeof(time_text), "%d:%04.2f", minutes, seconds);
    DrawText(time_text, 145, 5, 8, LIGHTGRAY);

    char lives_text[20];
    snprintf(lives_text, sizeof(lives_text), "<3 %d", player_lives);
    DrawText(lives_text, 295, 5, 8, LIGHTGRAY);

    char high_score_text[50];
    // Print high scores
    for (int i = 0; i < count; ++i)
    {
        snprintf(high_score_text, sizeof(high_score_text), "%s - %d", high_scores[i].username, high_scores[i].score);
        DrawText(high_score_text, 5, 10 * i + 16, 8, LIGHTGRAY);
    }

    DrawRectangle((int)paddle_x, paddle_y, paddle_size_x, paddle_size_y, RED);
    DrawTextureEx(ball_texture, ball_position, 1.0f, 0.5f, WHITE);
}

void playing_state_cleanup(void)
{
}
