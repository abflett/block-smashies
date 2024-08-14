#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "pause_menu_state.h"
#include "scene_manager.h"
#include "logo_scene.h"
#include "high_score.h"
#include "resource_manager.h"
#include "paddle.h"

// Paddle instance
static Paddle paddle;

// Ball properties
static Texture2D ball_texture;
static Vector2 ball_position;
static Vector2 ball_velocity;
static float ball_radius;

// Game status variables
static int player_lives;
static int player_score;
static float game_time;
static bool reset_game = false;

HighScore high_scores[10];
int count = 0;

GameState playing_state = {
    .init = playing_state_init,
    .update = playing_state_update,
    .render = playing_state_render,
    .cleanup = playing_state_cleanup,
};

void playing_state_init(void)
{
    if (reset_game)
    {
        // Reset game-specific variables without reloading resources
        paddle.reset(&paddle);

        ball_position = (Vector2){160.0f, 90.0f};   // Start in the middle of the screen
        ball_velocity = (Vector2){100.0f, -100.0f}; // Initial velocity
        ball_radius = 1.5f;

        reset_game = false; // Reset the flag after use
    }
    else if (!game_settings.is_paused)
    {
        // Full initialization, including resource loading
        paddle = create_paddle();

        ball_texture = resource_manager.get_texture("ball")->texture;

        // Load high scores from file
        load_high_scores("high_scores.json", high_scores, &count);

        // Add a new high score
        add_high_score(high_scores, &count, "Adam", 1000000);

        // Save high scores to file
        save_high_scores("high_scores.json", high_scores, count);

        // Full initialization logic
        ball_position = (Vector2){160.0f, 90.0f};   // Start in the middle of the screen
        ball_velocity = (Vector2){100.0f, -100.0f}; // Initial velocity
        ball_radius = 1.5f;

        player_lives = 3;
        player_score = 0;
        game_time = 0.0f;
    }
    else
    {
        game_settings.is_paused = false;
    }
}

void playing_state_update(float delta_time)
{
    // Update game time
    game_time += delta_time;

    paddle.update(&paddle, delta_time);

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
    if (ball_position.y + ball_radius >= paddle.position.y &&
        ball_position.x >= paddle.position.x &&
        ball_position.x <= paddle.position.x + paddle.size.x)
    {
        ball_velocity.y *= -1;                             // Reverse vertical direction
        ball_position.y = paddle.position.y - ball_radius; // Prevent the ball from sticking to the paddle

        // Adjust the ball's x-velocity based on the paddle's speed
        ball_velocity.x += paddle.speed * 0.5f; // Scale the influence of the paddle's speed

        // Ensure the ball's x-velocity doesn't exceed a certain maximum
        if (fabs(ball_velocity.x) > paddle.max_speed)
        {
            ball_velocity.x = (ball_velocity.x > 0) ? paddle.max_speed : -paddle.max_speed;
        }

        // Increase score when the ball hits the paddle
        player_score += 10;
    }

    // Ball falls below the paddle (you can add life loss or game over logic here)
    if (ball_position.y + ball_radius > game_settings.target_height)
    {
        player_lives--;
        reset_game = true;
        if (player_lives <= 0)
        {
            // Game over logic here
            playing_state_cleanup();
            scene_manager.change(&logo_scene);
        }
        else
        {
            // Reset ball position and velocity
            ball_position = (Vector2){160.0f, 90.0f};
            ball_velocity = (Vector2){100.0f, -100.0f};
        }
    }

    // Switch to the main menu scene if ENTER is pressed
    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.is_paused = true;
        game_state_manager.next_state = &pause_menu_state;
    }
}

void playing_state_render(void)
{
    char score_text[20];
    snprintf(score_text, sizeof(score_text), "%d", player_score);
    DrawText(score_text, 5, 5, 8, LIGHTGRAY);

    char time_text[20];
    int minutes = (int)(game_time / 60);
    float seconds = game_time - minutes * 60;
    snprintf(time_text, sizeof(time_text), "%d:%04.2f", minutes, seconds);
    DrawText(time_text, 145, 5, 8, LIGHTGRAY);

    char lives_text[20];
    snprintf(lives_text, sizeof(lives_text), "<3 %d", player_lives);
    DrawText(lives_text, 295, 5, 8, LIGHTGRAY);

    char high_score_text[50];
    for (int i = 0; i < count; ++i)
    {
        snprintf(high_score_text, sizeof(high_score_text), "%s - %d", high_scores[i].username, high_scores[i].score);
        DrawText(high_score_text, 5, 10 * i + 16, 8, LIGHTGRAY);
    }

    paddle.render(&paddle);
    DrawTextureEx(ball_texture, ball_position, 1.0f, 0.5f, WHITE);
}

void playing_state_cleanup(void)
{
    if (game_settings.is_paused)
        return;

    TraceLog(LOG_INFO, "playing_state_cleanup() called");
}
