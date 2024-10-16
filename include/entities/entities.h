#ifndef ENTITIES_H
#define ENTITIES_H

#define MAX_SHIPS 4

#include "box2d/box2d.h"
#include "ball.h"
#include "brick.h"
#include "kvec.h"
#include "game_data.h"
#include "wall_edges.h"
#include "kill_boundary.h"
#include "nanite.h"
#include "ship.h"

struct GameContext;

typedef struct Entities
{
    kvec_t(Ball *) balls;
    kvec_t(Brick *) bricks;
    kvec_t(Nanite *) nanites;

    Ship *ships[MAX_SHIPS];

    WallEdges *wall_edges;
    KillBoundary *kill_boundary;

    void (*add_ball)(GameData *game_data, b2WorldId world_id);
    void (*add_brick)(struct GameContext *game_context, b2Vec2 position, int brick_type);
    void (*add_nanite)(b2WorldId world_id, b2Vec2 position, float currency, int nanite_type);
    void (*update)(float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} Entities;

Entities *create_entities(struct GameContext *context);

#endif
