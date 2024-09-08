#include <stdlib.h>
#include <time.h>
#include "box2d/box2d.h"
#include "entities.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "player.h"
#include "game_settings.h"

static void add_ball_func(Entities *entities, Player *player, b2WorldId world_id, Paddle *paddle)
{
    float random_x = -100.0f + ((float)rand() / RAND_MAX) * 200.0f;

    b2Vec2 paddle_position = b2Body_GetPosition(paddle->body);

    // Check for inactive balls to reuse
    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *existing_ball = kv_A(entities->balls, i);
        if (!existing_ball->active)
        {
            existing_ball = create_ball(player, world_id,
                                        (b2Vec2){paddle_position.x, paddle_position.y + 3},
                                        (b2Vec2){random_x, 50});
            return;
        }
    }

    // If no inactive ball found, create a new one
    Ball *new_ball = create_ball(player, world_id,
                                 (b2Vec2){paddle_position.x, paddle_position.y + 3},
                                 (b2Vec2){random_x, 50});
    kv_push(Ball *, entities->balls, new_ball); // Push the pointer to kvec
}

static void add_paddle_func(Entities *entities, Player *player, b2WorldId world_id)
{
    // Todo: check active paddles before setting the player count
    // Check for inactive paddles to reuse
    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = kv_A(entities->paddles, i);
        if (!paddle->active)
        {
            paddle = create_paddle(i + 1, player, world_id);
            return;
        }
    }

    // If no inactive paddle found, create a new one
    Paddle *new_paddle = create_paddle((int)kv_size(entities->paddles) + 1, player, world_id);
    kv_push(Paddle *, entities->paddles, new_paddle); // Push the pointer to kvec
}

static void add_brick_func(Entities *entities, b2WorldId world_id, b2Vec2 position, int health)
{
    // Check for inactive bricks to reuse
    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *existing_brick = kv_A(entities->bricks, i);
        if (!existing_brick->active)
        {
            existing_brick->reset(existing_brick, position, health);
            return;
        }
    }

    // If no inactive brick found, create a new one
    Brick *new_brick = create_brick(world_id, position, health);
    kv_push(Brick *, entities->bricks, new_brick); // Push the pointer to kvec
}

static void add_wall_edges_func(Entities *entities, b2WorldId world_id)
{
    entities->wall_edges = create_wall_edges(world_id);
}

static void add_kill_boundary_func(Entities *entities, b2WorldId world_id)
{
    entities->kill_boundary = create_kill_boundary(world_id);
}

static void update_entities_func(Entities *entities, float delta_time)
{
    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = kv_A(entities->paddles, i);
        if (paddle->active)
        {
            paddle->update(paddle, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *paddle = kv_A(entities->balls, i);
        if (paddle->active)
        {
            paddle->update(paddle, delta_time);
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
    // Clean up wall edges
    if (entities->wall_edges && entities->wall_edges->clean_up)
    {
        entities->wall_edges->clean_up(entities->wall_edges);
    }

    // Clean up kill boundry
    if (entities->kill_boundary && entities->kill_boundary->clean_up)
    {
        entities->kill_boundary->clean_up(entities->kill_boundary);
    }

    // Clean up paddles
    for (size_t i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *paddle = kv_A(entities->paddles, i);
        if (paddle->clean_up)
        {
            paddle->clean_up(paddle);
        }
    }
    kv_destroy(entities->paddles); // Destroy the kvec itself

    // Clean up balls
    for (size_t i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *ball = kv_A(entities->balls, i);
        if (ball->clean_up)
        {
            ball->clean_up(ball);
        }
    }
    kv_destroy(entities->balls);

    // Clean up bricks
    for (size_t i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = kv_A(entities->bricks, i);
        if (brick->clean_up)
        {
            brick->clean_up(brick);
        }
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
    entities.add_wall_edges = add_wall_edges_func;
    entities.add_kill_boundary = add_kill_boundary_func;

    entities.update = update_entities_func;
    entities.render = render_entities_func;
    entities.cleanup = cleanup_entities_func;

    return entities;
}