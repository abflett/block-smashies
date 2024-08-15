#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "game_over_state.h"
#include "pause_menu_state.h"
#include "game_status.h"
#include "entities.h"

Entities entities;

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
        entities.paddle = create_paddle();
        entities.ball = create_ball((Vector2){160.0f, 90.0f});

        // paddle = create_paddle();
        // ball = create_ball((Vector2){160.0f, 90.0f});
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
    entities.paddle.update(&entities.paddle, delta_time);
    entities.ball.update(&entities.ball, delta_time);

    // Ball collision with paddle
    if (entities.ball.position.y + entities.ball.radius >= entities.paddle.position.y &&
        entities.ball.position.x >= entities.paddle.position.x &&
        entities.ball.position.x <= entities.paddle.position.x + entities.paddle.size.x)
    {
        entities.ball.velocity.y *= -1;                             // Reverse vertical direction
        entities.ball.position.y = entities.paddle.position.y - entities.ball.radius; // Prevent the ball from sticking to the paddle

        // Adjust the ball's x-velocity based on the paddle's speed
        entities.ball.velocity.x += entities.paddle.speed * 0.5f; // Scale the influence of the paddle's speed

        // Ensure the ball's x-velocity doesn't exceed a certain maximum
        if (fabs(entities.ball.velocity.x) > entities.paddle.max_speed)
        {
            entities.ball.velocity.x = (entities.ball.velocity.x > 0) ? entities.paddle.max_speed : -entities.paddle.max_speed;
        }

        // Increase score when the ball hits the paddle
        game_status.score += 10;
    }

    // Ball falls below the screen (you can add life loss or game over logic here)
    if (entities.ball.position.y > game_settings.target_height)
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
            entities.ball.reset(&entities.ball, (Vector2){160.0f, 90.0f});
            entities.paddle.reset(&entities.paddle);
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
    entities.paddle.render(&entities.paddle);
    entities.ball.render(&entities.ball);
}

void playing_state_cleanup(void)
{
    if (game_settings.is_paused)
        return;
    TraceLog(LOG_INFO, "playing_state_cleanup() called");
}