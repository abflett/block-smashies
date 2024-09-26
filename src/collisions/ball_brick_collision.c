#include "ball_brick_collision.h"
#include "kvec.h"

void ball_brick_collision(Ball *ball, Brick *brick, GameContext *context)
{
    brick->health -= *ball->power;
    if (brick->health <= 0)
    {
        brick->disable(brick);
        context->shake_effect->start(1.5f, 0.5f);
        context->game_status->score += brick->brick_type * 10;
    }
}
