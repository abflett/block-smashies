#include <stddef.h>
#include "play_state_manager.h"
#include "play_state.h"

void change_state_global(PlayState *new_state)
{
    if (play_state_manager.current_state && play_state_manager.current_state->cleanup)
    {
        play_state_manager.current_state->cleanup();
    }

    play_state_manager.current_state = new_state;
    if (play_state_manager.current_state && play_state_manager.current_state->init)
    {
        play_state_manager.current_state->init();
    }
}

void update_state_global(float delta_time)
{
    if (play_state_manager.next_state)
    {
        change_state_global(play_state_manager.next_state);
        play_state_manager.next_state = NULL; // Clear the next_state pointer
    }

    if (play_state_manager.current_state && play_state_manager.current_state->update)
    {
        play_state_manager.current_state->update(delta_time);
    }
}

void render_state_global(void)
{
    if (play_state_manager.current_state && play_state_manager.current_state->render)
    {
        play_state_manager.current_state->render();
    }
}

PlayStateManager play_state_manager = {
    .current_state = NULL,
    .next_state = NULL,
    .change_state = change_state_global,
    .update_state = update_state_global,
    .render_state = render_state_global};