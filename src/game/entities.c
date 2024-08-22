#include <stdlib.h>
#include <time.h>
#include "entities.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "player.h"

static void add_ball_func(Entities *entities, Player *player, Paddle *paddle)
{
    float random_x = -50.0f + ((float)rand() / RAND_MAX) * 100.0f;

    // Check for inactive balls to reuse
    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *existing_ball = kv_A(entities->balls, i);
        if (!existing_ball->active)
        {
            *existing_ball = create_ball(player,
                                         (Vector2){paddle->position.x + (paddle->size.x / 2),
                                                   paddle->position.y - 3},
                                         (Vector2){random_x, -100});
            return;
        }
    }

    // If no inactive ball found, create a new one
    Ball *new_ball = malloc(sizeof(Ball));
    *new_ball = create_ball(player,
                            (Vector2){paddle->position.x + (paddle->size.x / 2),
                                      paddle->position.y - 3},
                            (Vector2){random_x, -100});
    kv_push(Ball *, entities->balls, new_ball); // Push the pointer to kvec
}

static void add_paddle_func(Entities *entities, Player *player)
{
    // Check for inactive paddles to reuse
    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = kv_A(entities->paddles, i);
        if (!paddle->active)
        {
            *paddle = create_paddle(i + 1, player);
            return;
        }
    }

    // If no inactive paddle found, create a new one
    Paddle *new_paddle = malloc(sizeof(Paddle));
    *new_paddle = create_paddle(kv_size(entities->paddles) + 1, player);
    kv_push(Paddle *, entities->paddles, new_paddle); // Push the pointer to kvec
}

static void add_brick_func(Entities *entities, Brick brick)
{
    // Check for inactive bricks to reuse
    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *existing_brick = kv_A(entities->bricks, i);
        if (!existing_brick->active)
        {
            *existing_brick = brick;
            existing_brick->active = true;
            return;
        }
    }

    // If no inactive brick found, create a new one
    Brick *new_brick = malloc(sizeof(Brick));
    *new_brick = brick;
    new_brick->active = true;
    kv_push(Brick *, entities->bricks, new_brick); // Push the pointer to kvec
}

static void update_entities_func(Entities *entities, float delta_time)
{
    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *ball = kv_A(entities->balls, i);
        if (ball->active)
        {
            ball->update(ball, entities, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = kv_A(entities->paddles, i);
        if (paddle->active)
        {
            paddle->update(paddle, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = kv_A(entities->bricks, i);
        if (brick->active)
        {
            brick->update(brick, entities, delta_time);
        }
    }

    entities->game_status.update(&entities->game_status, delta_time);
}

static void render_entities_func(Entities *entities)
{
    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *ball = kv_A(entities->balls, i);
        if (ball->active)
        {
            ball->render(ball);
        }
    }

    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = kv_A(entities->paddles, i);
        if (paddle->active)
        {
            paddle->render(paddle);
        }
    }

    // Render bricks
    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = kv_A(entities->bricks, i);
        if (brick->active)
        {
            brick->render(brick);
        }
    }

    entities->game_status.render(&entities->game_status);
}

static void cleanup_entities_func(Entities *entities)
{
    // Clean up paddles
    for (size_t i = 0; i < kv_size(entities->paddles); i++)
    {
        free(kv_A(entities->paddles, i)); // Free each dynamically allocated Paddle
    }
    kv_destroy(entities->paddles); // Destroy the kvec itself

    // Clean up balls
    for (size_t i = 0; i < kv_size(entities->balls); i++)
    {
        free(kv_A(entities->balls, i)); // Free each dynamically allocated Ball
    }
    kv_destroy(entities->balls);

    // Clean up bricks
    for (size_t i = 0; i < kv_size(entities->bricks); i++)
    {
        free(kv_A(entities->bricks, i)); // Free each dynamically allocated Brick
    }
    kv_destroy(entities->bricks);
}

Entities create_entities()
{
    Entities entities;
    kv_init(entities.balls);
    kv_init(entities.paddles);
    kv_init(entities.bricks);
    entities.game_status = create_game_status();

    entities.add_ball = add_ball_func;
    entities.add_paddle = add_paddle_func;
    entities.add_brick = add_brick_func;
    entities.update = update_entities_func;
    entities.render = render_entities_func;
    entities.cleanup = cleanup_entities_func;

    return entities;
}
