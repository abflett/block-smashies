#include <stdlib.h>
#include "box2d/box2d.h"
#include "entities.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "nanite.h"
#include "game_data.h"
#include "settings.h"
#include "game_context.h"

Entities entities;

static void add_ball_func(GameData *game_data, b2WorldId world_id, Paddle *paddle)
{
    float random_x = (float)GetRandomValue(-25, 25);
    b2Vec2 paddle_position = b2Body_GetPosition(paddle->body);

    for (int i = 0; i < kv_size(entities.balls); i++)
    {
        Ball *existing_ball = kv_A(entities.balls, i);
        if (!existing_ball->active)
        {
            existing_ball->reset(existing_ball,
                                 (b2Vec2){paddle_position.x, paddle_position.y + (paddle->size.y / 2) + 3},
                                 (b2Vec2){random_x, 50});
            return;
        }
    }

    Ball *new_ball = create_ball(game_data, world_id,
                                 (b2Vec2){paddle_position.x, paddle_position.y + (paddle->size.y / 2) + 3},
                                 (b2Vec2){random_x, 50});
    kv_push(Ball *, entities.balls, new_ball);
}

static void add_paddle_func(GameData *game_data, b2WorldId world_id)
{
    int paddle_count = (int)kv_size(entities.paddles);
    int player_count = 0;

    for (int i = 0; i < kv_size(entities.paddles); i++)
    {
        Paddle *existing_paddle = kv_A(entities.paddles, i);
        if (existing_paddle->active)
        {
            player_count++;
        }
    }

    for (int i = 0; i < paddle_count; i++)
    {
        Paddle *existing_paddle = kv_A(entities.paddles, i);
        if (!existing_paddle->active)
        {
            existing_paddle->reset(existing_paddle, player_count);
            return;
        }
    }

    // Todo: check active paddles before setting the player count
    Paddle *new_paddle = create_paddle(player_count, game_data, world_id);
    kv_push(Paddle *, entities.paddles, new_paddle);
}

static void add_brick_func(GameContext *game_context, b2Vec2 position, int brick_type)
{
    for (int i = 0; i < kv_size(game_context->entities->bricks); i++)
    {
        Brick *existing_brick = kv_A(game_context->entities->bricks, i);
        if (!existing_brick->active)
        {
            existing_brick->reset(existing_brick, position, brick_type);
            return;
        }
    }

    Brick *new_brick = create_brick(game_context, position, brick_type);
    kv_push(Brick *, entities.bricks, new_brick);
}

static void add_nanite_func(b2WorldId world_id, b2Vec2 position, float currency, int nanite_type)
{
    for (int i = 0; i < kv_size(entities.nanites); i++)
    {
        Nanite *existing_nanite = kv_A(entities.nanites, i);
        if (!existing_nanite->active)
        {
            existing_nanite->reset(existing_nanite, position, currency, nanite_type);
            return;
        }
    }

    Nanite *new_nanite = create_nanite(world_id, position, currency, nanite_type);
    kv_push(Nanite *, entities.nanites, new_nanite);
}

static void add_wall_edges_func(b2WorldId world_id)
{
    entities.wall_edges = create_wall_edges(world_id);
}

static void add_kill_boundary_func(b2WorldId world_id)
{
    entities.kill_boundary = create_kill_boundary(world_id);
}

static void update_entities_func(float delta_time)
{
    for (int i = 0; i < kv_size(entities.paddles); i++)
    {
        Paddle *paddle = kv_A(entities.paddles, i);
        if (paddle->active)
        {
            paddle->update(paddle, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities.balls); i++)
    {
        Ball *paddle = kv_A(entities.balls, i);
        if (paddle->active)
        {
            paddle->update(paddle, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities.nanites); i++)
    {
        Nanite *nanite = kv_A(entities.nanites, i);
        if (nanite->active)
        {
            nanite->update(nanite, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities.bricks); i++)
    {
        Brick *brick = kv_A(entities.bricks, i);
        if (brick->active)
        {
            brick->update(brick, delta_time);
        }
    }
}

static void render_entities_func(void)
{
    for (int i = 0; i < kv_size(entities.balls); i++)
    {
        Ball *ball = kv_A(entities.balls, i);
        if (ball->active)
        {
            ball->render(ball);
        }
    }

    for (int i = 0; i < kv_size(entities.paddles); i++)
    {
        Paddle *paddle = kv_A(entities.paddles, i);
        if (paddle->active)
        {
            paddle->render(paddle);
        }
    }

    for (int i = 0; i < kv_size(entities.bricks); i++)
    {
        Brick *brick = kv_A(entities.bricks, i);
        if (brick->active)
        {
            brick->render(brick);
        }
    }

    for (int i = 0; i < kv_size(entities.nanites); i++)
    {
        Nanite *nanite = kv_A(entities.nanites, i);
        if (nanite->active)
        {
            nanite->render(nanite);
        }
    }
}

static void cleanup_entities_func(void)
{
    // Clean up wall edges
    if (entities.wall_edges && entities.wall_edges->clean_up)
    {
        entities.wall_edges->clean_up(entities.wall_edges);
    }

    // Clean up kill boundry
    if (entities.kill_boundary && entities.kill_boundary->clean_up)
    {
        entities.kill_boundary->clean_up(entities.kill_boundary);
    }

    // Clean up paddles
    for (size_t i = 0; i < kv_size(entities.paddles); i++)
    {
        Paddle *paddle = kv_A(entities.paddles, i);
        if (paddle->clean_up)
        {
            paddle->clean_up(paddle);
        }
    }
    kv_destroy(entities.paddles);

    // Clean up balls
    for (size_t i = 0; i < kv_size(entities.balls); i++)
    {
        Ball *ball = kv_A(entities.balls, i);
        if (ball->clean_up)
        {
            ball->clean_up(ball);
        }
    }
    kv_destroy(entities.balls);

    // Clean up bricks
    for (size_t i = 0; i < kv_size(entities.bricks); i++)
    {
        Brick *brick = kv_A(entities.bricks, i);
        if (brick->clean_up)
        {
            brick->clean_up(brick);
        }
    }
    kv_destroy(entities.bricks);

    // Clean up nanites
    for (size_t i = 0; i < kv_size(entities.nanites); i++)
    {
        Nanite *nanite = kv_A(entities.nanites, i);
        if (nanite->clean_up)
        {
            nanite->clean_up(nanite);
        }
    }
    kv_destroy(entities.nanites);
}

Entities *create_entities()
{
    kv_init(entities.balls);
    kv_init(entities.paddles);
    kv_init(entities.bricks);
    kv_init(entities.nanites);

    entities.add_ball = add_ball_func;
    entities.add_paddle = add_paddle_func;
    entities.add_brick = add_brick_func;
    entities.add_nanite = add_nanite_func;
    entities.add_wall_edges = add_wall_edges_func;
    entities.add_kill_boundary = add_kill_boundary_func;

    entities.update = update_entities_func;
    entities.render = render_entities_func;
    entities.cleanup = cleanup_entities_func;

    return &entities;
}
