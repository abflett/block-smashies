#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "game_over_state.h"
#include "pause_menu_state.h"
#include "game_status.h"
#include "paddle.h"
#include "ball.h"

static Paddle paddle;
static Ball ball;
static GameStatus game_status;

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
        paddle = create_paddle();
        ball = create_ball((Vector2){160.0f, 90.0f});
        game_status = create_game_status();
    }
    else
    {
        game_settings.is_paused = false;
    }
}

void playing_state_update(float delta_time)
{
    game_status.update(&game_status, delta_time);
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
        game_status.score += 10;
    }

    // Ball falls below the screen (you can add life loss or game over logic here)
    if (ball.position.y > game_settings.target_height)
    {
        game_status.lives--;
        if (game_status.lives <= 0)
        {
            playing_state_cleanup();
            game_status.reset(&game_status);
            game_state_manager.change(&game_over_state);
        }
        else
        {
            ball.reset(&ball, (Vector2){160.0f, 90.0f});
            paddle.reset(&paddle);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.is_paused = true;
        game_state_manager.next_state = &pause_menu_state;
    }
}

void playing_state_render(void)
{
    game_status.render(&game_status);
    paddle.render(&paddle);
    ball.render(&ball);
}

void playing_state_cleanup(void)
{
    if (game_settings.is_paused)
        return;
    TraceLog(LOG_INFO, "playing_state_cleanup() called");
}