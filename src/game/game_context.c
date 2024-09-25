#include <stdlib.h>
#include "game_context.h"
#include "resource_manager.h"
#include "game.h"

static GameContext context;

static void update_game_context(float delta_time)
{
    static float accumulator = 0.0f;
    float timeStep = 1.0f / 60.0f;
    int subStepCount = 4;
    accumulator += delta_time;
    while (accumulator >= timeStep)
    {
        b2World_Step(context.world_id, timeStep, subStepCount);
        accumulator -= timeStep;
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
    TraceLog(LOG_INFO, "[Cleanup] - WorldId [%d] - Success", context.world_id.index1);
    b2DestroyWorld(context.world_id);
}

GameContext *create_game_context(GameData *game_data)
{
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = (b2Vec2){0.0f, 0.0f};

    context.world_id = b2CreateWorld(&world_def);
    context.collision_manager = create_collision_manager(context.world_id);
    context.game_data = game_data;
    context.game_status = create_game_status(context.game_data->player_count);
    context.game_ui = create_game_ui(context.game_status);
    context.entities = create_entities();
    context.shake_effect = get_shake_effect();

    context.entities->add_wall_edges(context.world_id);
    context.entities->add_kill_boundary(context.world_id);

    context.update = update_game_context;
    context.render = render_game_context;
    context.cleanup = cleanup_game_context;

    return &context;
}
