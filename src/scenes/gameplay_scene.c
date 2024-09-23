#include "raylib.h"
#include "gameplay_scene.h"
#include "game_state_manager.h"
#include "settings.h"
#include "game_context.h"

static Scene gameplay_scene;
static GameContext *context;

static void scene_init(void)
{
    context = create_game_context();
    game_state_manager.context = context;

    // load player, this will define players level and stats based on new game, loaded game and player count
    // setup paddle default positions and initial ball.
    // use the context level loader to create the level and populate the playfield
    context->entities->add_paddle(&context->player, context->world_id);
    Paddle *paddle = kv_A(context->entities->paddles, 0); // player1 paddle

    for (int i = 0; i < 1; i++)
    {
        context->entities->add_ball(&context->player, context->world_id, paddle);
    }

    int brick_row = 12;
    int brick_column = 12;
    float row_spacing = 18.0f; // Horizontal spacing (width of the brick + any gap)
    float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)
    for (int col = 0; col < brick_row; col++)
    {
        for (int row = 0; row < brick_column; row++)
        {
            int brick_type = rand() % 100;
            if (brick_type < 25)
            {
                context->entities->add_brick(context, (b2Vec2){row * row_spacing + settings.game.play_area.x + 10, settings.game.target_size.y - (col * col_spacing + settings.game.play_area.y + 5)}, brick_type);
            }
        }
    }

    // start cutscene and cutsene will change to playing once animation is finished.
    game_state_manager.change(game_state_manager.states.game_intro_start);
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
    TraceLog(LOG_INFO, "[Cleanup] - scene_cleanup - Success");
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