#include "raylib.h"
#include "box2d/box2d.h"
#include "kvec.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "entities.h"
#include "resource_manager.h"
#include "player.h"
#include "wall_edges.h"
#include "kill_boundary.h"
#include "collision_manager.h"

static Entities entities;
static Texture2D *background;
static Player player;
static bool is_hold;
static WallEdges *wall_edges;
static KillBoundary *kill_boundary;
static b2WorldId world_id;
static CollisionManager collision_manager;

static void state_init(int argc, va_list args)
{
    if (!game_settings.is_paused)
    {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = (b2Vec2){0.0f, 0.0f};
        world_id = b2CreateWorld(&worldDef);
        collision_manager = create_collision_manager(world_id);

        wall_edges = create_wall_edges(world_id);
        kill_boundary = create_kill_boundary(world_id);

        is_hold = true;

        background = &resource_manager.get_texture("gameplay-bg")->texture;
        player = create_new_player("Player 1");
        entities = create_entities();

        entities.add_paddle(&entities, &player, world_id);

        Paddle *paddle = kv_A(entities.paddles, 0); // player1 paddle

        for (int i = 0; i < 1; i++)
        {
            entities.add_ball(&entities, &player, world_id, paddle);
        }

        int brick_row = 12;
        int brick_column = 12;
        float row_spacing = 18.0f; // Horizontal spacing (width of the brick + any gap)
        float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)
        for (int col = 0; col < brick_row; col++)
        {
            for (int row = 0; row < brick_column; row++)
            {
                entities.add_brick(&entities, world_id, (b2Vec2){row * row_spacing + game_settings.play_area.x + 10, game_settings.target_height - (col * col_spacing + game_settings.play_area.y + 5)}, 1);
            }
        }
    }
    else
    {
        game_settings.is_paused = false;
    }
}

static void state_cleanup(void)
{
    if (!game_settings.is_paused)
    {
        TraceLog(LOG_INFO, "[Cleanup] - playing_state - Success");
        entities.cleanup(&entities);
        TraceLog(LOG_INFO, "[Cleanup] - Wall_Edges [%d] - Success", wall_edges->body.index1);
        wall_edges->clean_up(wall_edges);
        TraceLog(LOG_INFO, "[Cleanup] - Kill_Boundry [%d] - Success", kill_boundary->body.index1);
        kill_boundary->clean_up(kill_boundary);

        b2DestroyWorld(world_id);
    }
}

static void state_update(float delta_time)
{
    if (!is_hold || IsKeyPressed(KEY_SPACE))
    {
        is_hold = false;
        entities.update(&entities, delta_time);

        if (entities.game_status.lives <= 0)
        {
            game_state_manager.change(game_state_manager.states.game_over, 1, entities.game_status.score);
        }

        // box2d physics
        static float accumulator = 0.0f;
        float timeStep = 1.0f / 60.0f;
        int subStepCount = 4;
        accumulator += delta_time;
        while (accumulator >= timeStep)
        {
            b2World_Step(world_id, timeStep, subStepCount);
            accumulator -= timeStep;
            collision_manager.process_collisions(&collision_manager);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.is_paused = true;
        game_state_manager.change(game_state_manager.states.pause_menu, 1, &entities);
        return;
    }
}

static void state_render(void)
{
    DrawTexture(*background, 0, 0, WHITE);
    entities.render(&entities);
}

GameState playing_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};