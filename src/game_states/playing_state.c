#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "game_status.h"
#include "entities.h"

Entities entities;

static GameStatus game_status;

static void state_init(int argc, va_list args)
{
    if (!game_settings.is_paused)
    {
        entities.paddle = create_paddle();
        entities.ball = create_ball((Vector2){160.0f, 90.0f});
        game_status = create_game_status();
    }
    else
    {
        game_settings.is_paused = false;
    }
}

static void state_cleanup(void)
{
    if (game_settings.is_paused)
        return;
    TraceLog(LOG_INFO, "state_cleanup() called");
}

static void state_update(float delta_time)
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
            state_cleanup();
            game_state_manager.change(game_state_manager.states.game_over, 1, game_status.score);
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
        game_state_manager.change(game_state_manager.states.pause_menu, 0);
    }
}

static void state_render(void)
{
    game_status.render(&game_status);
    entities.paddle.render(&entities.paddle);
    entities.ball.render(&entities.ball);
}

GameState playing_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};