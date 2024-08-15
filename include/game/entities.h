#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include "paddle.h"
#include "ball.h"


typedef struct Entities {
    Paddle paddle;
    Ball ball;

} Entities;


#endif