#include "gameplay.h"
#include "scene_manager.h"
#include "playing_state.h"
#include "play_state_manager.h"
#include "main_menu.h"

extern SceneManager scene_manager;

Scene gameplay_scene = {
    .init = gameplay_init,
    .update = gameplay_update,
    .render = gameplay_render,
    .cleanup = gameplay_cleanup,
};

PlayStateManager play_state_manager;

void gameplay_init(void)
{
    play_state_manager_change_state(&play_state_manager, &playing_state);
}

void gameplay_update(float delta_time)
{

    play_state_manager_update(&play_state_manager, delta_time);
}

void gameplay_render(void)
{

    play_state_manager_render(&play_state_manager);
}

void gameplay_cleanup(void)
{
    if (play_state_manager.current_state && play_state_manager.current_state->cleanup)
    {
        play_state_manager.current_state->cleanup();
    }
}
