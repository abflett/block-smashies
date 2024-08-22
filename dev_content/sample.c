#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "kvec.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "game_status.h"
#include "entities.h"

static Entities entities;

static GameStatus game_status;

static void state_init(int argc, va_list args)
{
    if (!game_settings.is_paused)
    {
        // Initialize the entities if not already initialized
        entities = create_entities();
        entities.add_paddle(&entities, create_paddle());

        // Add a ball to the entities
        entities.add_ball(&entities, create_ball((Vector2){160.0f, 90.0f}));

        // Initialize the game status
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

    entities.cleanup(&entities);
}

static void state_update(float delta_time)
{
    game_status.update(&game_status, delta_time);

    // Update all entities
    entities.update(&entities, delta_time);

    // Example collision detection for the first ball and paddle
    Ball *ball = kv_A(entities.balls, 0);
    Paddle *paddle = kv_A(entities.paddles, 0);

    if (ball->position.y + ball->radius >= paddle->position.y &&
        ball->position.x >= paddle->position.x &&
        ball->position.x <= paddle->position.x + paddle->size.x)
    {
        ball->velocity.y *= -1;                               // Reverse vertical direction
        ball->position.y = paddle->position.y - ball->radius; // Prevent the ball from sticking to the paddle

        // Adjust the ball's x-velocity based on the paddle's speed
        ball->velocity.x += paddle->speed * 0.5f; // Scale the influence of the paddle's speed

        // Ensure the ball's x-velocity doesn't exceed a certain maximum
        if (fabs(ball->velocity.x) > paddle->max_speed)
        {
            ball->velocity.x = (ball->velocity.x > 0) ? paddle->max_speed : -paddle->max_speed;
        }

        // Increase score when the ball hits the paddle
        game_status.score += 10;
    }

    // Ball falls below the screen (you can add life loss or game over logic here)
    if (ball->position.y > game_settings.target_height)
    {
        game_status.lives--;
        if (game_status.lives <= 0)
        {
            state_cleanup();
            game_state_manager.change(game_state_manager.states.game_over, 1, game_status.score);
        }
        else
        {
            ball->reset(ball, (Vector2){160.0f, 90.0f});
            paddle->reset(paddle);
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

    entities.render(&entities);
}

GameState playing_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};