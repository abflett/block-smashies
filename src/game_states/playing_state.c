#include "raylib.h"
#include "kvec.h"
#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "entities.h"
#include "resource_manager.h"
#include "player.h"

static Entities entities;
static Texture2D *background;
static Player player;
static bool is_hold;

static void state_init(int argc, va_list args)
{

    if (!game_settings.is_paused)
    {
        is_hold = true;

        background = &resource_manager.get_texture("gameplay-bg")->texture;
        player = create_new_player("Player 1");
        entities = create_entities();

        entities.add_paddle(&entities, &player);
        entities.add_paddle(&entities, &player);

        Paddle *paddle = &kv_A(entities.paddles, 0); // player1 paddle
        for (int i = 0; i < 2000; i++)
        {
            entities.add_ball(&entities, &player, paddle);
        }

        int brick_row = 10;
        int brick_column = 10;

        float row_spacing = 20.0f; // Horizontal spacing (width of the brick + any gap)
        float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)

        for (int col = 0; col < brick_row; col++)
        {
            for (int row = 0; row < brick_column; row++)
            {
                entities.add_brick(&entities, create_brick((Vector2){
                                                  row * row_spacing + game_settings.play_area.x,
                                                  col * col_spacing + game_settings.play_area.y}));
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
        entities.cleanup(&entities);
        game_settings.is_paused = false;
        TraceLog(LOG_INFO, "[Cleanup] - playing_state - Success");
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

        if (IsKeyPressed(KEY_ESCAPE))
        {
            game_settings.is_paused = true;
            game_state_manager.change(game_state_manager.states.pause_menu, 1, &entities);
        }
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