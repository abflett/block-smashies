#include "nanite_kill_boundary_collision.h"

void nanite_kill_boundary_collision(Nanite *nanite, KillBoundary *kill_boundary, GameContext *GameContext)
{
    nanite->disable(nanite);
    TraceLog(LOG_ERROR, "Disable nanite boundry");
}
