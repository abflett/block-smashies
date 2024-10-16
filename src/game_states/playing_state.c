#include <stdlib.h>
#include "raylib.h"
#include "box2d/box2d.h"
#include "kvec.h"
#include "playing_state.h"
#include "settings.h"
#include "game_state_manager.h"
#include "game_context.h"
#include "playing_input_handler.h"

static GameState playing_state;
static PlayingInputHandler *input_handler;

static void state_init(void)
{
    GameContext *context = game_state_manager.context;
    input_handler = initialize_playing_input(context);

    // game is playing, activate physics for all ships
    Ship **ships = context->entities->ships;
    for (int i = 0; i < context->game_data->player_count; i++)
    {
        if (ships[i] != NULL && !ships[i]->physics_active && ships[i]->active)
        {
            ships[i]->activate_ship_physics(ships[i], context);
        }
    }

    if (context->game_status->is_pause)
    {
        context->game_status->is_pause = false;
    }
}

static void state_cleanup(void)
{
}

static void state_update(float delta_time)
{
    input_handler->update(delta_time);
    game_state_manager.context->update(delta_time);

    if (game_state_manager.context->game_status->enemy_count <= 0)
    {
        game_state_manager.context->game_data->mission++;
        game_state_manager.context->level_manager->load_level();
        game_state_manager.context->level_manager->create_level();

        TraceLog(LOG_INFO, "Loading - Operation: %d, Mission %d", game_state_manager.context->game_data->operation, game_state_manager.context->game_data->mission);
    }

    if (game_state_manager.context->game_status->game_over)
    {
        game_state_manager.change(game_state_manager.states.game_over);
    }
}

static void state_render(void)
{
    game_state_manager.context->render();
}

GameState *create_playing_state()
{
    playing_state.init = state_init;
    playing_state.update = state_update;
    playing_state.render = state_render;
    playing_state.cleanup = state_cleanup;

    return &playing_state;
};