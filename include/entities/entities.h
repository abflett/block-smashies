#ifndef ENTITIES_H
#define ENTITIES_H

#include "box2d/box2d.h"
#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include "game_status.h"
#include "kvec.h"
#include "player.h"
#include "wall_edges.h"
#include "kill_boundary.h"

typedef kvec_t(Ball *) BallVector;
typedef kvec_t(Paddle *) PaddleVector;
typedef kvec_t(Brick *) BrickVector;

typedef struct Entities
{
    BallVector balls;
    PaddleVector paddles;
    BrickVector bricks;
    GameStatus game_status;
    WallEdges *wall_edges;
    KillBoundary *kill_boundary;

    void (*add_ball)(struct Entities *entities, Player *player, b2WorldId world_id, Paddle *paddle);
    void (*add_paddle)(struct Entities *entities, Player *player, b2WorldId world_id);
    void (*add_brick)(struct Entities *entities, b2WorldId world_id, b2Vec2 position, int health);
    void (*add_wall_edges)(struct Entities *entities, b2WorldId world_id);
    void (*add_kill_boundary)(struct Entities *entities, b2WorldId world_id);
    void (*update)(struct Entities *entities, float delta_time);
    void (*render)(struct Entities *entities);
    void (*cleanup)(struct Entities *entities);
} Entities;

Entities create_entities();

#endif
