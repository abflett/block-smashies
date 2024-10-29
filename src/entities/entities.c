#include <stdlib.h>
#include "entities.h"
#include "settings.h"
#include "game_context.h"

#define MAX_SHIPS 4

Entities entities;

static void add_ball(void)
{
    const float random_x = (float)GetRandomValue(-25, 25);
    const Ship *p1_ship = entities.ships[0];

    const b2Vec2 ship_position = p1_ship->position;

    for (int i = 0; i < kv_size(entities.balls); i++)
    {
        Ball *existing_ball = kv_A(entities.balls, i);
        if (!existing_ball->active)
        {
            existing_ball->reset(existing_ball,
                                 (b2Vec2){ship_position.x, ship_position.y + (p1_ship->shield_size.y) + 3},
                                 (b2Vec2){random_x, 50});
            return;
        }
    }

    Ball *new_ball = create_ball(entities.game_context->game_data, entities.game_context->world_id,
                                 (b2Vec2){ship_position.x, ship_position.y + (p1_ship->shield_size.y) + 3},
                                 (b2Vec2){random_x, 50});

    kv_push(Ball *, entities.balls, new_ball);
}

static void add_ships(void)
{
    const GameContext *context = entities.game_context;

    const float play_area_center = settings.game.play_area.width * 0.5f + settings.game.play_area.x;
    const float ship_spacing = settings.game.play_area.width / (float)(context->game_data->player_count + 1); // Spacing based on player count

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        const float x_position = play_area_center - (float)(context->game_data->player_count - 1) * ship_spacing / 2.0f + (float)i * ship_spacing;
        if (entities.ships[i] == NULL)
        {
            entities.ships[i] = create_ship(&context->game_data->ships[i].player_num,
                                            context->game_data,
                                            (b2Vec2){
                                                x_position,
                                                19.0f});
        }

        // set active if game_data has the ship as active for up to 4 players
        entities.ships[i]->active = context->game_data->ships[i].active;
    }
}

static void add_brick(const b2Vec2 position, const int brick_type)
{
    for (int i = 0; i < kv_size(entities.bricks); i++)
    {
        Brick *existing_brick = kv_A(entities.bricks, i);
        if (!existing_brick->active)
        {
            existing_brick->reset(existing_brick, position, brick_type);
            return;
        }
    }

    Brick *new_brick = create_brick(entities.game_context, position, brick_type);
    kv_push(Brick *, entities.bricks, new_brick);
}

static void add_nanite(const b2Vec2 position, const float currency, const int nanite_type)
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

    Nanite *new_nanite = create_nanite(entities.game_context->world_id, position, currency, nanite_type);
    kv_push(Nanite *, entities.nanites, new_nanite);
}

static void add_wall_edges(void)
{
    entities.wall_edges = create_wall_edges(entities.game_context->world_id);
}

static void add_kill_boundary(void)
{
    entities.kill_boundary = create_kill_boundary(entities.game_context->world_id);
}

static void update_entities(const float delta_time)
{
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

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        if (entities.ships[i]->active)
        {
            entities.ships[i]->update(entities.ships[i], delta_time);
        }
    }
}

static void render_entities(void)
{
    for (int i = 0; i < kv_size(entities.balls); i++)
    {
        const Ball *ball = kv_A(entities.balls, i);
        if (ball->active)
        {
            ball->render(ball);
        }
    }

    for (int i = 0; i < kv_size(entities.bricks); i++)
    {
        const Brick *brick = kv_A(entities.bricks, i);
        if (brick->active)
        {
            brick->render(brick);
        }
    }

    for (int i = 0; i < kv_size(entities.nanites); i++)
    {
        const Nanite *nanite = kv_A(entities.nanites, i);
        if (nanite->active)
        {
            nanite->render(nanite);
        }
    }

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        if (entities.ships[i]->active)
        {
            entities.ships[i]->render(entities.ships[i]);
        }
    }
}

static void cleanup_entities(void)
{
    // Clean up wall edges
    if (entities.wall_edges && entities.wall_edges->clean_up)
    {
        entities.wall_edges->clean_up(entities.wall_edges);
    }

    // Clean up kill boundary
    if (entities.kill_boundary && entities.kill_boundary->clean_up)
    {
        entities.kill_boundary->clean_up(entities.kill_boundary);
    }

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

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        entities.ships[i]->cleanup(entities.ships[i]);
        entities.ships[i] = NULL;
    }
}

Entities *create_entities(GameContext *context)
{
    entities.game_context = context;
    kv_init(entities.balls);
    kv_init(entities.bricks);
    kv_init(entities.nanites);

    add_ships();
    add_wall_edges();
    add_kill_boundary();

    entities.add_ball = add_ball;
    entities.add_brick = add_brick;
    entities.add_nanite = add_nanite;

    entities.update = update_entities;
    entities.render = render_entities;
    entities.cleanup = cleanup_entities;

    return &entities;
}
