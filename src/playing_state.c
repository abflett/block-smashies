#include "playing_state.h"
#include "play_state_manager.h"

extern PlayStateManager play_state_manager;

PlayState playing_state = {
    .init = playing_state_init,
    .update = playing_state_update,
    .render = playing_state_render,
    .cleanup = playing_state_cleanup,
};

void playing_state_init(void)
{
}

void playing_state_update(float delta_time)
{
}

void playing_state_render(void)
{
}

void playing_state_cleanup(void)
{
}
