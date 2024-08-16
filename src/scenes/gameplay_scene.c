#include "gameplay_scene.h"
#include "game_state_manager.h"
#include "playing_state.h"


void gameplay_scene_init(void)
{
    game_state_manager.change(&playing_state);
}

void gameplay_scene_update(float delta_time)
{

    game_state_manager.update(delta_time);
}

void gameplay_scene_render(void)
{

    game_state_manager.render();
}

void gameplay_scene_cleanup(void)
{
    if (game_state_manager.current_state && game_state_manager.current_state->cleanup)
    {
        game_state_manager.current_state->cleanup();
    }
}

Scene gameplay_scene = {
    .init = gameplay_scene_init,
    .update = gameplay_scene_update,
    .render = gameplay_scene_render,
    .cleanup = gameplay_scene_cleanup,
};