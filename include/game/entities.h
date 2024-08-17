#ifndef ENTITIES_H
#define ENTITIES_H

#include "paddle.h"
#include "ball.h"
#include "kvec.h"

typedef kvec_t(Ball) BallVector;
typedef kvec_t(Paddle) PaddleVector;

typedef struct Entities {
    BallVector balls;       // Dynamic array of balls
    PaddleVector paddles;   // Dynamic array of paddles

    // Function pointers for entity operations
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