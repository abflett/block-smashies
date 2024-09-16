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
    if (game_state_manager.context->game_status.is_pause)
    {
        game_state_manager.context->game_status.is_pause = false;
    }
}

static void state_cleanup(void)
{
}

static void state_update(float delta_time)
{
    if (!game_state_manager.context->game_status.is_hold || IsKeyPressed(KEY_SPACE))
    {
        game_state_manager.context->game_status.is_hold = false;
        game_state_manager.context->update(delta_time);
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_state_manager.context->game_status.is_pause = true;
        game_state_manager.change(game_state_manager.states.pause_menu);
        return;
    }

    if (game_state_manager.context->game_status.game_over)
    {
        game_state_manager.change(game_state_manager.states.game_over);
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