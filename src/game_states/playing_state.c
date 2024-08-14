#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "game_over_state.h"
#include "pause_menu_state.h"
#include "paddle.h"
#include "ball.h"

// Paddle instance
static Paddle paddle;

// Ball instance
static Ball ball;

// Game status variables
static int player_lives;
static int player_score;
static float game_time;
static bool reset_game = false;

GameState playing_state = {
    .init = playing_state_init,
    .update = playing_state_update,
    .render = playing_state_render,
    .cleanup = playing_state_cleanup,
};

void playing_state_init(void)
{
    if (!game_settings.is_paused)
    {
        // Full initialization, including resource loading
        paddle = create_paddle();
        ball = create_ball((Vector2){160.0f, 90.0f}); // Initialize ball at the center of the screen

        // Initialize game status variables
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
    ball.update(&ball, delta_time);

    // Ball collision with paddle
    if (ball.position.y + ball.radius >= paddle.position.y &&
        ball.position.x >= paddle.position.x &&
        ball.position.x <= paddle.position.x + paddle.size.x)
    {
        ball.velocity.y *= -1;                             // Reverse vertical direction
        ball.position.y = paddle.position.y - ball.radius; // Prevent the ball from sticking to the paddle

        // Adjust the ball's x-velocity based on the paddle's speed
        ball.velocity.x += paddle.speed * 0.5f; // Scale the influence of the paddle's speed

        // Ensure the ball's x-velocity doesn't exceed a certain maximum
        if (fabs(ball.velocity.x) > paddle.max_speed)
        {
            ball.velocity.x = (ball.velocity.x > 0) ? paddle.max_speed : -paddle.max_speed;
        }

        // Increase score when the ball hits the paddle
        player_score += 10;
    }

    // Ball falls below the paddle (you can add life loss or game over logic here)
    if (ball.position.y + ball.radius > game_settings.target_height)
    {
        player_lives--;
        reset_game = true;
        if (player_lives <= 0)
        {
            // Game over logic here
            playing_state_cleanup();
            game_state_manager.change(&game_over_state);
        }
        else
        {
            // Reset ball position and velocity
            ball.reset(&ball, (Vector2){160.0f, 90.0f});
            paddle.reset(&paddle);
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

    paddle.render(&paddle);
    ball.render(&ball);
}

void playing_state_cleanup(void)
{
    if (game_settings.is_paused)
        return;

    TraceLog(LOG_INFO, "playing_state_cleanup() called");
}
