#ifndef ENTITIES_H
#define ENTITIES_H

#include "paddle.h"
#include "ball.h"
#include "game_status.h"
#include "kvec.h"

typedef kvec_t(Ball) BallVector;
typedef kvec_t(Paddle) PaddleVector;

typedef struct Entities {
    BallVector balls;
    PaddleVector paddles;
    GameStatus game_status;

    void (*add_ball)(struct Entities *entities, Ball ball);
    void (*add_paddle)(struct Entities *entities, Paddle paddle);
    void (*remove_ball)(struct Entities *entities, int index);
    void (*remove_paddle)(struct Entities *entities, int index);
    void (*update)(struct Entities *entities, float delta_time);
    void (*render)(struct Entities *entities);
    void (*cleanup)(struct Entities *entities);
} Entities;

Entities create_entities();

#endif