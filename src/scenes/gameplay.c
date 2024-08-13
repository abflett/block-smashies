#include "gameplay.h"
#include "game_state_manager.h"
#include "playing_state.h"

Scene gameplay_scene = {
    .init = gameplay_init,
    .update = gameplay_update,
    .render = gameplay_render,
    .cleanup = gameplay_cleanup,
};

void gameplay_init(void)
{
    game_state_manager.change(&playing_state);
}

void gameplay_update(float delta_time)
{

    game_state_manager.update(delta_time);
}

void gameplay_render(void)
{

    game_state_manager.render();
}

void gameplay_cleanup(void)
{
    if (game_state_manager.current_state && game_state_manager.current_state->cleanup)
    {
        game_state_manager.current_state->cleanup();
    }
}
