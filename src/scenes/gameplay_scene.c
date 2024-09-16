#include "raylib.h"
#include "gameplay_scene.h"
#include "game_state_manager.h"
#include "game_settings.h"
#include "game_context.h"

static GameContext *context;

static void scene_init(void)
{
    context = create_game_context();
    game_state_manager.context = context;

    game_state_manager.change(game_state_manager.states.playing);
}

static void scene_update(float delta_time)
{
    game_state_manager.update(delta_time);
}

static void scene_render(void)
{
    game_state_manager.render();
}

static void scene_cleanup(void)
{
    if (game_state_manager.current_state && game_state_manager.current_state->cleanup)
    {
        game_state_manager.current_state->cleanup();
        TraceLog(LOG_INFO, "[Cleanup] - gameplay_scene - Success");
    }

    // cleanup the playing state if playing state paused
    if (game_state_manager.context->game_status.is_pause)
    {
        game_state_manager.context->game_status.is_pause = false;
        if (game_state_manager.states.playing->cleanup)
        {
            game_state_manager.states.playing->cleanup();
        }
    }
}

Scene gameplay_scene = {
    .init = scene_init,
    .update = scene_update,
    .render = scene_render,
    .cleanup = scene_cleanup,
};