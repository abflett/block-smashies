#include "gameplay.h"
#include "play_state_manager.h"
#include "playing_state.h"

Scene gameplay_scene = {
    .init = gameplay_init,
    .update = gameplay_update,
    .render = gameplay_render,
    .cleanup = gameplay_cleanup,
};

void gameplay_init(void)
{
    play_state_manager.change_state(&playing_state);
}

void gameplay_update(float delta_time)
{

    play_state_manager.update_state(delta_time);
}

void gameplay_render(void)
{

    play_state_manager.render_state();
}

void gameplay_cleanup(void)
{
    if (play_state_manager.current_state && play_state_manager.current_state->cleanup)
    {
        play_state_manager.current_state->cleanup();
    }
}
