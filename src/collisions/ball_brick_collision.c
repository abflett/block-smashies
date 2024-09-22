#include "ball_brick_collision.h"

void ball_brick_collision(Ball *ball, Brick *brick, GameContext *context)
{
    brick->health -= *ball->power;
    if (brick->health <= 0)
    {
        b2Vec2 position = b2Body_GetPosition(brick->body);

        brick->disable(brick);

        TraceLog(LOG_INFO, "Brick Position - X[%f], Y[%f]", position.x, position.y);
        context->shake_effect->start(1.5f, 0.5f);
    }
    TraceLog(LOG_INFO, "Begin Contact - Ball[%d], Brick[%d]", ball->body.index1, brick->body.index1);
}
