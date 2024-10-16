#include "nanite_paddle_collision.h"

void nanite_paddle_collision(Nanite *nanite, GameContext *context)
{
    context->game_status->currency += nanite->currency;
    nanite->disable(nanite);
}
