#include "nanite_paddle_collision.h"

void nanite_paddle_collision(Nanite *nanite, Paddle *paddle, GameContext *context)
{
    context->game_status->currency += nanite->currency;
    nanite->disable(nanite);
    TraceLog(LOG_INFO, "Begin Contact - Nanite[%d], Paddle[%d]", nanite->body.index1, paddle->body.index1);
}
