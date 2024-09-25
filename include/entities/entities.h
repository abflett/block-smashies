#ifndef ENTITIES_H
#define ENTITIES_H

#include "box2d/box2d.h"
#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include "kvec.h"
#include "game_data.h"
#include "wall_edges.h"
#include "kill_boundary.h"
#include "nanite.h"

struct GameContext;

typedef struct Entities
{
    kvec_t(Ball *) balls;
    kvec_t(Paddle *) paddles;
    kvec_t(Brick *) bricks;
    kvec_t(Nanite *) nanites;
    WallEdges *wall_edges;
    KillBoundary *kill_boundary;

    void (*add_ball)(GameData *player, b2WorldId world_id, Paddle *paddle);
    void (*add_paddle)(GameData *player, b2WorldId world_id);
    void (*add_brick)(struct GameContext *game_context, b2Vec2 position, int brick_type);
    void (*add_nanite)(b2WorldId world_id, b2Vec2 position, float currency);
    void (*add_wall_edges)(b2WorldId world_id);
    void (*add_kill_boundary)(b2WorldId world_id);
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} Entities;

Entities *create_entities();

#endif
