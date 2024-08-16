#include "gameplay_scene.h"
#include "game_state_manager.h"
#include "playing_state.h"

static void scene_init(void)
{
    game_state_manager.change(&playing_state);
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
    }
}

Scene gameplay_scene = {
    .init = scene_init,
    .update = scene_update,
    .render = scene_render,
    .cleanup = scene_cleanup,
};