
#include "ball_kill_boundary_collision.h"
#include "kill_boundary.h"

void ball_kill_boundary_collision(Ball *ball, KillBoundary *kill_boundary, GameContext *context)
{
    TraceLog(LOG_INFO, "Begin Contact - Ball[%d], KillBoundary[%d]", ball->body.index1, kill_boundary->body.index1);
}
