#ifndef ENTITIES_H
#define ENTITIES_H

#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include "game_status.h"
#include "kvec.h"
#include "player.h"

typedef kvec_t(Ball) BallVector;
typedef kvec_t(Paddle) PaddleVector;
typedef kvec_t(Brick) BrickVector;

typedef struct Entities
{
    BallVector balls;
    PaddleVector paddles;
    BrickVector bricks;
    GameStatus game_status;

    void (*add_ball)(struct Entities *entities, Player *player, Paddle *paddle);
    void (*add_paddle)(struct Entities *entities, Player *player);
    void (*add_brick)(struct Entities *entities, Brick brick);
    void (*remove_ball)(struct Entities *entities, int index);
    void (*remove_paddle)(struct Entities *entities, int index);
    void (*update)(struct Entities *entities, float delta_time);
    void (*render)(struct Entities *entities);
    void (*cleanup)(struct Entities *entities);
} Entities;

Entities create_entities();

#endif