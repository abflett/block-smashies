#include "game_context.h"

static GameContext context;
static const float TIME_STEP = 1.0f / 60.0f;
static const int SUB_STEP_COUNT = 4;

static void update_game_context(const float delta_time)
{
    static float accumulator = 0.0f;
    accumulator += delta_time;
    while (accumulator >= TIME_STEP)
    {
        b2World_Step(context.world_id, TIME_STEP, SUB_STEP_COUNT);
        accumulator -= TIME_STEP;
        context.collision_manager->process_collisions(&context);
    }

    context.entities->update(delta_time);
    context.game_status->update(delta_time);
    context.game_ui->update(delta_time);
}

static void render_game_context(void)
{
    context.game_ui->render_before_content();
    context.entities->render();
    context.game_ui->render_after_content();
}

static void cleanup_game_context(void)
{
    context.entities->cleanup();
    context.game_ui->cleanup();
    b2DestroyWorld(context.world_id);
}

GameContext *create_game_context(GameData *game_data)
{
    // world setup
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = (b2Vec2){0.0f, 0.0f};
    context.world_id = b2CreateWorld(&world_def);
    context.collision_manager = create_collision_manager(context.world_id);

    // game setup
    context.game_data = game_data;

    context.shake_effect = get_shake_effect();
    context.game_status = create_game_status(context.game_data->player_count);
    context.game_ui = create_game_ui(&context);
    context.entities = create_entities(&context);
    context.level_manager = create_level_manager(&context);

    context.update = update_game_context;
    context.render = render_game_context;
    context.cleanup = cleanup_game_context;

    return &context;
}
