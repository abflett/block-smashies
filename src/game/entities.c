#include <stdlib.h>
#include "entities.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "player.h"

static void add_ball_func(Entities *entities, Ball ball)
{
    // Check for an inactive ball slot first
    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *existing_ball = &kv_A(entities->balls, i);
        if (!existing_ball->active)
        {
            // Reuse this inactive slot
            *existing_ball = ball; // Todo: reset ball instead of creating a new;
            existing_ball->active = true;
            return;
        }
    }

    // If no inactive slot was found, add a new ball to the array
    kv_push(Ball, entities->balls, ball);
}

static void add_paddle_func(Entities *entities, Player *player)
{
    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (!paddle->active)
        {
            *paddle = create_paddle(i + 1, player); // Assign to the existing Paddle struct
            return;                                 // Exit the function after reusing an inactive paddle
        }
    }

    kv_push(Paddle, entities->paddles, create_paddle(kv_size(entities->paddles) + 1, player));
}

static void add_brick_func(Entities *entities, Brick brick)
{
    // Check for an inactive brick slot first
    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *existing_brick = &kv_A(entities->bricks, i);
        if (!existing_brick->active)
        {
            // Reuse this inactive slot
            *existing_brick = brick; // Replace the inactive brick with the new brick data
            existing_brick->active = true;
            return;
        }
    }

    // If no inactive slot was found, add a new brick to the array
    kv_push(Brick, entities->bricks, brick);
}

static void remove_ball_func(Entities *entities, int index)
{
    if (index >= 0 && index < kv_size(entities->balls))
    {
        kv_A(entities->balls, index).active = false;
    }
}

static void remove_paddle_func(Entities *entities, int index)
{
    if (index >= 0 && index < kv_size(entities->paddles))
    {
        kv_A(entities->paddles, index).active = false;
    }
}

static void update_entities_func(Entities *entities, float delta_time)
{
    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *ball = &kv_A(entities->balls, i);
        if (ball->active)
        {
            ball->update(ball, entities, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (paddle->active)
        {
            paddle->update(paddle, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = &kv_A(entities->bricks, i);
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
        Ball *ball = &kv_A(entities->balls, i);
        if (ball->active)
        {
            ball->render(ball);
        }
    }

    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = &kv_A(entities->paddles, i);
        if (paddle->active)
        {
            paddle->render(paddle);
        }
    }

    // Render bricks
    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = &kv_A(entities->bricks, i);
        if (brick->active)
        {
            brick->render(brick);
        }
    }

    entities->game_status.render(&entities->game_status);
}

static void cleanup_entities_func(Entities *entities)
{
    kv_destroy(entities->balls);
    kv_destroy(entities->paddles);
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
    entities.remove_ball = remove_ball_func;
    entities.remove_paddle = remove_paddle_func;
    entities.update = update_entities_func;
    entities.render = render_entities_func;
    entities.cleanup = cleanup_entities_func;

    return entities;
}
