#include "ball_brick_collision.h"

void ball_brick_collision(const Ball *ball, Brick *brick, const GameContext *context)
{
    brick->health -= *ball->orb_power;
    if (brick->health <= 0)
    {
        brick->disable(brick);
        context->shake_effect->start(1.5f, 0.5f);
        context->game_status->score += (brick->brick_type + 1) * 10;
    }
}
