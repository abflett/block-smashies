#include "playing_state.h"
#include "game_settings.h"
#include "game_state_manager.h"
#include "entities.h"

static Entities entities;

static void state_init(int argc, va_list args)
{
    if (!game_settings.is_paused)
    {
        entities = create_entities();
        entities.add_paddle(&entities, create_paddle());
        entities.add_ball(&entities, create_ball((Vector2){160.0f, 160.0f}));
        entities.add_ball(&entities, create_ball((Vector2){160.0f, 170.0f}));

        int brick_row = 6;
        int brick_column = 10;

        float margin_left = 50.0f;
        float margin_top = 20.0f;
        float row_spacing = 20.0f; // Horizontal spacing (width of the brick + any gap)
        float col_spacing = 8.0f;  // Vertical spacing (height of the brick + any gap)

        for (int col = 0; col < brick_row; col++)
        {
            for (int row = 0; row < brick_column; row++)
            {
                entities.add_brick(&entities, create_brick((Vector2){
                                                  row * row_spacing + margin_left,
                                                  col * col_spacing + margin_top}));
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
    entities.update(&entities, delta_time);

    if (entities.game_status.lives <= 0)
    {
        game_state_manager.change(game_state_manager.states.game_over, 1, entities.game_status.score);
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_settings.is_paused = true;
        game_state_manager.change(game_state_manager.states.pause_menu, 0);
    }
}

static void state_render(void)
{
    entities.render(&entities);
}

GameState playing_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};