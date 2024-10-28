#include "nanite_ship_collision.h"

void nanite_ship_collision(Nanite *nanite, const GameContext *context)
{
    context->game_status->currency += nanite->currency;
    nanite->disable(nanite);
}
