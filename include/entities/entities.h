#ifndef ENTITIES_H
#define ENTITIES_H

#define MAX_SHIPS 4

#include "box2d/box2d.h"
#include "ball.h"
#include "brick.h"
#include "ship.h"
#include "wall_edges.h"
#include "kill_boundary.h"
#include "nanite.h"
#include "kvec.h"
#include "game_data.h"


struct GameContext;

typedef struct Entities
{
    struct GameContext *game_context;

    kvec_t(Ball *) balls;
    kvec_t(Brick *) bricks;
    kvec_t(Nanite *) nanites;
    Ship *ships[MAX_SHIPS];
    WallEdges *wall_edges;
    KillBoundary *kill_boundary;

    void (*add_ball)(void);
    void (*add_brick)(const b2Vec2 position, const int brick_type);
    void (*add_nanite)(const b2Vec2 position, const float currency, const int nanite_type);
    void (*update)(const float delta_time);
    void (*render)(void);
    void (*cleanup)(void);
} Entities;

Entities *create_entities(struct GameContext *context);

#endif
