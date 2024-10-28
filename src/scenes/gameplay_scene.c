#include <stdarg.h>
#include "raylib.h"
#include "gameplay_scene.h"
#include "game_state_manager.h"
#include "settings.h"
#include "game_context.h"

static Scene gameplay_scene;
static GameContext *context;

static void scene_init(const int arg_count, const va_list args)
{
    GameData *game_data = NULL;

    for (int i = 0; i < arg_count; i++)
    {
        if (i == 0)
            game_data = va_arg(args, GameData *);
    }

    context = create_game_context(game_data);
    game_state_manager.context = context;
    context->level_manager->create_level();
    context->entities->add_ball();

    // start cutscene and cutscene will change to playing once animation is finished.
    game_state_manager.change(game_state_manager.states.game_intro_start, 0, NULL);
}

static void scene_update(const float delta_time)
{
    game_state_manager.update(delta_time);
}

static void scene_render(void)
{
    game_state_manager.render();
}

static void scene_cleanup(void)
{
    TraceLog(LOG_INFO, "[Cleanup] - scene_cleanup");
    if (game_state_manager.current_state && game_state_manager.current_state->cleanup)
    {
        game_state_manager.current_state->cleanup();
    }

    // cleanup the playing state if playing state paused
    if (game_state_manager.context->game_status->is_pause)
    {
        game_state_manager.context->game_status->is_pause = false;
        if (game_state_manager.states.playing->cleanup)
        {
            game_state_manager.states.playing->cleanup();
        }
    }

    game_state_manager.context->cleanup();
}

Scene *create_gameplay_scene()
{
    gameplay_scene.init = scene_init;
    gameplay_scene.update = scene_update;
    gameplay_scene.render = scene_render;
    gameplay_scene.cleanup = scene_cleanup;

    return &gameplay_scene;
};