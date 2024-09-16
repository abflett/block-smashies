#include <stdlib.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "kvec.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "game_context.h"

static void state_init(void)
{
    if (!game_state_manager.context->game_status.is_pause)
    {
        game_state_manager.context = create_game_context();
        game_state_manager.context->entities.add_paddle(&game_state_manager.context->entities, &game_state_manager.context->player, game_state_manager.context->world_id);
        Paddle *paddle = kv_A(game_state_manager.context->entities.paddles, 0); // player1 paddle

        for (int i = 0; i < 1; i++)
        {
            game_state_manager.context->entities.add_ball(&game_state_manager.context->entities, &game_state_manager.context->player, game_state_manager.context->world_id, paddle);
        }

        int brick_row = 12;
        int brick_column = 12;
        float row_spacing = 18.0f; // Horizontal spacing (width of the brick + any gap)
        float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)
        for (int col = 0; col < brick_row; col++)
        {
            for (int row = 0; row < brick_column; row++)
            {
                int brick_type = rand() % 50;
                if (brick_type <= 25)
                {
                    game_state_manager.context->entities.add_brick(&game_state_manager.context->entities, game_state_manager.context->world_id, (b2Vec2){row * row_spacing + game_settings.play_area.x + 10, game_settings.target_height - (col * col_spacing + game_settings.play_area.y + 5)}, brick_type);
                }
            }
        }
    }
    else
    {
        game_state_manager.context->game_status.is_pause = false;
    }
}

static void state_cleanup(void)
{
    if (!game_state_manager.context->game_status.is_pause)
    {
        game_state_manager.context->cleanup();
    }
}

static void state_update(float delta_time)
{
    if (!game_state_manager.context->game_status.is_hold || IsKeyPressed(KEY_SPACE))
    {
        game_state_manager.context->game_status.is_hold = false;
        game_state_manager.context->update(delta_time);

        if (game_state_manager.context->game_status.game_over)
        {
            game_state_manager.change(game_state_manager.states.game_over);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_state_manager.context->game_status.is_pause = true;
        game_state_manager.change(game_state_manager.states.pause_menu);
        return;
    }
}

static void state_render(void)
{
    game_state_manager.context->render();
}

GameState playing_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};