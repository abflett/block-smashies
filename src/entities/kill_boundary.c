#include "kill_boundary.h"

static void clean_up_kill_boundary(KillBoundary *kill_boundary)
{
    b2DestroyBody(kill_boundary->body);
}

KillBoundary create_kill_boundary(b2WorldId world_id)
{
    KillBoundary kill_boundary;
    kill_boundary.clean_up = clean_up_kill_boundary;

    return kill_boundary;
}