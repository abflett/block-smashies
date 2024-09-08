#include <stdlib.h>
#include "game_context.h"
#include "resource_manager.h"

void update_game_context(GameContext *context, float delta_time)
{
    context->entities.update(&context->entities, delta_time);

    // box2d physics
    static float accumulator = 0.0f;
    float timeStep = 1.0f / 60.0f;
    int subStepCount = 4;
    accumulator += delta_time;
    while (accumulator >= timeStep)
    {
        b2World_Step(context->world_id, timeStep, subStepCount);
        accumulator -= timeStep;
        context->collision_manager.process_collisions(&context->collision_manager, context);
    }
}

void render_game_context(GameContext *context)
{
    DrawTexture(*context->background, 0, 0, WHITE);
    context->entities.render(&context->entities);
}

void cleanup_game_context(GameContext *context)
{
    context->entities.cleanup(&context->entities);
    TraceLog(LOG_INFO, "[Cleanup] - WorldId [%d] - Success", context->world_id.index1);
    b2DestroyWorld(context->world_id);
    free(context);
}

GameContext *create_game_context()
{
    GameContext *context = (GameContext *)malloc(sizeof(GameContext));

    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = (b2Vec2){0.0f, 0.0f};
    context->world_id = b2CreateWorld(&world_def);
    context->collision_manager = create_collision_manager(context->world_id);

    context->is_hold = true;

    context->background = &resource_manager.get_texture("gameplay-bg")->texture;
    context->player = create_new_player("Player 1");
    context->entities = create_entities();

    context->entities.add_wall_edges(&context->entities, context->world_id);
    context->entities.add_kill_boundary(&context->entities, context->world_id);

    context->update = update_game_context;
    context->render = render_game_context;
    context->cleanup = cleanup_game_context;

    return context;
}
