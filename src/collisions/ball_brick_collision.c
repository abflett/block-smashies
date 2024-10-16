#include "ball_brick_collision.h"
#include "kvec.h"
#include "settings.h"

void ball_brick_collision(Ball *ball, Brick *brick, GameContext *context)
{
    brick->health -= *ball->power;
    if (brick->health <= 0)
    {
        brick->disable(brick);
        context->shake_effect->start(1.5f, 0.5f);
        context->game_status->score += brick->brick_type * 10;

        int active_bricks = 0;
        for (int i = 0; i < kv_size(context->entities->bricks); i++)
        {
            Brick *brick = kv_A(context->entities->bricks, i);
            if (brick->active)
            {
                active_bricks++;
            }
        }

        TraceLog(LOG_INFO, "Active Bricks: %d", active_bricks);

        if (active_bricks == 1)
        {
            int brick_row = 12;
            int brick_column = 12;
            float row_spacing = 18.0f; // Horizontal spacing (width of the brick + any gap)
            float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)
            for (int col = 0; col < brick_row; col++)
            {
                for (int row = 0; row < brick_column; row++)
                {
                    int brick_type = GetRandomValue(0, 50);
                    if (brick_type < 25)
                    {
                        context->entities->add_brick((b2Vec2){row * row_spacing + settings.game.play_area.x + 10, settings.game.target_size.y - (col * col_spacing + settings.game.play_area.y + 5)}, brick_type);
                    }
                }
            }
        }
    }
}
