
#include "kvec.h"
#include "box2d/box2d.h"

#include "ball_kill_boundary_collision.h"
#include "kill_boundary.h"

void ball_kill_boundary_collision(Ball *ball, KillBoundary *kill_boundary, GameContext *context)
{
    // Disable the ball (set its active status to false)
    ball->disable(ball);

    // Check how many balls are still active
    int active_ball_count = 0;
    for (int i = 0; i < kv_size(context->entities.balls); i++)
    {
        Ball *existing_ball = kv_A(context->entities.balls, i);
        if (existing_ball->active)
        {
            active_ball_count++;
        }
    }

    // If no active balls remain, decrement lives
    if (active_ball_count == 0)
    {
        context->game_status.lives--;

        // Check if lives are exhausted, and set game over if true
        if (context->game_status.lives <= 0)
        {
            context->game_status.game_over = true;
        }
        else
        {
            // get the first paddle;
            Paddle *paddle = kv_A(context->entities.paddles, 0);
            paddle->reset(paddle, paddle->player_num);
            context->entities.add_ball(&context->entities, &context->player, context->world_id, paddle);
            context->game_status.is_hold = true;
        }
    }
    TraceLog(LOG_INFO, "Begin Contact - Ball[%d], KillBoundary[%d]", ball->body.index1, kill_boundary->body.index1);
}
