#include "raylib.h"
#include "box2d/box2d.h"
#include "kvec.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "entities.h"

#include "game_context.h"

static GameContext *context;

static void state_init(int argc, va_list args)
{
    if (!game_settings.is_paused)
    {
        context = create_game_context();
        context->entities.add_paddle(&context->entities, &context->player, context->world_id);
        Paddle *paddle = kv_A(context->entities.paddles, 0); // player1 paddle

        for (int i = 0; i < 1; i++)
        {
            context->entities.add_ball(&context->entities, &context->player, context->world_id, paddle);
        }

        int brick_row = 12;
        int brick_column = 12;
        float row_spacing = 18.0f; // Horizontal spacing (width of the brick + any gap)
        float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)
        for (int col = 0; col < brick_row; col++)
        {
            for (int row = 0; row < brick_column; row++)
            {
                context->entities.add_brick(&context->entities, context->world_id, (b2Vec2){row * row_spacing + game_settings.play_area.x + 10, game_settings.target_height - (col * col_spacing + game_settings.play_area.y + 5)}, 1);
            }
        }
    }
    else
    {
        game_settings.is_paused = false;
    }
}

static void state_cleanup(void)
{
    if (!game_settings.is_paused)
    {
        context->cleanup(context);
    }
}

static void state_update(float delta_time)
{
    if (!context->is_hold || IsKeyPressed(KEY_SPACE))
    {
        context->is_hold = false;
        context->update(context, delta_time);

        if (context->entities.game_status.lives <= 0)
        {
            game_state_manager.change(game_state_manager.states.game_over, 1, context->entities.game_status.score);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.is_paused = true;
        game_state_manager.change(game_state_manager.states.pause_menu, 1, &context->entities);
        return;
    }
}

static void state_render(void)
{
    context->render(context);
}

GameState playing_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};