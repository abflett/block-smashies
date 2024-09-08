#include "ball_brick_collision.h"

void ball_brick_collision(Ball *ball, Brick *brick, GameContext *context)
{
    brick->health -= 1;
    if (brick->health <= 0)
    {
        brick->disable(brick);
    }
    TraceLog(LOG_INFO, "Begin Contact - Ball[%d], Brick[%d]", ball->body.index1, brick->body.index1);
}
