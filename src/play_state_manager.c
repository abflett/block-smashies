#include <stddef.h>
#include "play_state_manager.h"
#include "play_state.h"

void play_state_manager_change_state(PlayStateManager *manager, PlayState *new_state)
{
    if (manager->current_state && manager->current_state->cleanup)
    {
        manager->current_state->cleanup();
    }

    manager->current_state = new_state;
    if (manager->current_state && manager->current_state->init)
    {
        manager->current_state->init();
    }
}

void play_state_manager_update(PlayStateManager *manager, float delta_time)
{
    if (manager->next_state)
    {
        play_state_manager_change_state(manager, manager->next_state);
        manager->next_state = NULL; // Clear the next_state pointer
    }

    if (manager->current_state && manager->current_state->update)
    {
        manager->current_state->update(delta_time);
    }
}

void play_state_manager_render(PlayStateManager *manager)
{
    if (manager->current_state && manager->current_state->render)
    {
        manager->current_state->render();
    }
}