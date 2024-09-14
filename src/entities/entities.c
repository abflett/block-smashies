#include <stdlib.h>
#include <time.h>
#include "box2d/box2d.h"
#include "entities.h"
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "nanite.h"
#include "player.h"
#include "game_settings.h"

static void add_ball_func(Entities *entities, Player *player, b2WorldId world_id, Paddle *paddle)
{
    float random_x = -100.0f + ((float)rand() / RAND_MAX) * 200.0f;
    b2Vec2 paddle_position = b2Body_GetPosition(paddle->body);

    for (int i = 0; i < kv_size(entities->balls); i++)
    {
        Ball *existing_ball = kv_A(entities->balls, i);
        if (!existing_ball->active)
        {
            existing_ball->reset(existing_ball,
                                 (b2Vec2){paddle_position.x, paddle_position.y + (paddle->size.y / 2) + 4},
                                 (b2Vec2){random_x, 50});
            return;
        }
    }

    Ball *new_ball = create_ball(player, world_id,
                                 (b2Vec2){paddle_position.x, paddle_position.y + (paddle->size.y / 2) + 4},
                                 (b2Vec2){random_x, 50});
    kv_push(Ball *, entities->balls, new_ball);
}

static void add_paddle_func(Entities *entities, Player *player, b2WorldId world_id)
{
    // Todo: check active paddles before setting the player count
    for (int i = 0; i < kv_size(entities->paddles); i++)
    {
        Paddle *existing_paddle = kv_A(entities->paddles, i);
        if (!existing_paddle->active)
        {
            existing_paddle->reset(existing_paddle, 1);
            return;
        }
    }

    // Todo: check active paddles before setting the player count
    Paddle *new_paddle = create_paddle((int)kv_size(entities->paddles) + 1, player, world_id);
    kv_push(Paddle *, entities->paddles, new_paddle);
}

static void add_brick_func(Entities *entities, b2WorldId world_id, b2Vec2 position, BrickType brick_type)
{
    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *existing_brick = kv_A(entities->bricks, i);
        if (!existing_brick->active)
        {
            existing_brick->reset(existing_brick, position, brick_type);
            return;
        }
    }

    Brick *new_brick = create_brick(entities, world_id, position, brick_type);
    kv_push(Brick *, entities->bricks, new_brick);
}

static void add_nanite_func(Entities *entities, b2WorldId world_id, b2Vec2 position, int currency)
{
    TraceLog(LOG_INFO, "Adding new nanite - world_id: %d, pos.x: %f, pos.y: %f, currency: %d", world_id.index1, position.x, position.y, currency);
    for (int i = 0; i < kv_size(entities->nanites); i++)
    {
        Nanite *existing_nanite = kv_A(entities->nanites, i);
        if (!existing_nanite->active)
        {
            existing_nanite->reset(existing_nanite, position, currency);
            return;
        }
    }

    Nanite *new_nanite = create_nanite(world_id, position, currency);
    kv_push(Nanite *, entities->nanites, new_nanite);
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

    for (int i = 0; i < kv_size(entities->nanites); i++)
    {
        Nanite *nanite = kv_A(entities->nanites, i);
        if (nanite->active)
        {
            nanite->update(nanite, delta_time);
        }
    }

    for (int i = 0; i < kv_size(entities->bricks); i++)
    {
        Brick *brick = kv_A(entities->bricks, i);
        if (brick->active)
        {
            brick->update(brick, delta_time);
        }
    }
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

    for (int i = 0; i < kv_size(entities->nanites); i++)
    {
        Nanite *nanite = kv_A(entities->nanites, i);
        if (nanite->active)
        {
            nanite->render(nanite);
        }
    }
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
    kv_destroy(entities->paddles);

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

    // Clean up nanites
    for (size_t i = 0; i < kv_size(entities->nanites); i++)
    {
        Nanite *nanite = kv_A(entities->nanites, i);
        if (nanite->clean_up)
        {
            nanite->clean_up(nanite);
        }
    }
    kv_destroy(entities->nanites);
}

Entities create_entities()
{
    Entities entities;
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

    return entities;
}
