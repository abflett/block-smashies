#include "ball_brick_collision.h"

void ball_brick_collision(Ball *ball, Brick *brick, GameContext *context)
{
    brick->health -= *ball->power;
    if (brick->health <= 0)
    {
        b2Vec2 position = b2Body_GetPosition(brick->body);
        brick->disable(brick);

        context->shake_effect->start(1.5f, 0.5f);
        context->game_status->score += brick->brick_type * 10;
    }
}
