#include <stdio.h>
#include <math.h>
#include "raylib.h"

#include "game_intro_start_state.h"
#include "game_state_manager.h"
#include "settings.h"

static GameState game_intro_start_state;
static GameContext *context;
static Ship **ships;
static Font *font;
static float count_down;
static float y_position;

static void state_init(const int arg_count, va_list args)
{
    context = game_state_manager.context;

    count_down = 3.0f;
    y_position = 3.0f;
    ships = context->entities->ships;

    for (int i = 0; i < context->game_data->player_count; i++)
    {
        ships[i]->position = (b2Vec2){ships[i]->position.x, y_position};
    }
}

static void state_update(const float delta_time)
{
    y_position += delta_time * 5.7f;
    for (int i = 0; i < context->game_data->player_count; i++)
    {
        ships[i]->position = (b2Vec2){ships[i]->position.x, y_position};
    }

    count_down -= delta_time;

    if (count_down < 0)
    {
        game_state_manager.change(game_state_manager.states.playing, 0, NULL);
    }

    game_state_manager.context->game_ui->update(delta_time);
}

static void state_render(void)
{
    game_state_manager.context->render();

    // Round up the floating-point number
    const int rounded_count_down = (int)ceil((double)count_down);

    const char *countdown_text = TextFormat("%d", rounded_count_down);

    const float middle_x = (settings.game.play_area.width * 0.5f) + settings.game.play_area.x;

    const Vector2 contact_size = MeasureTextEx(*font, "Contact in", 7, 0.0f);
    const Vector2 countdown_size = MeasureTextEx(*font, countdown_text, 7, 0.0f);

    // Draw countdown text
    DrawTextEx(*font, "Contact in", (Vector2){middle_x - contact_size.x * 0.5f, 115}, 7, 0.0f, WHITE);
    DrawTextEx(*font, countdown_text, (Vector2){middle_x - countdown_size.x * 0.5f - 1, 125}, 14, 0.0f, WHITE);
}

static void state_cleanup(void)
{
    // Cleanup logic for the intro state
}

// Return a pointer to the statically defined GameState
GameState *create_game_intro_start_state(void)
{
    font = resource_manager.get_pixel7_font();

    game_intro_start_state.init = state_init;
    game_intro_start_state.update = state_update;
    game_intro_start_state.render = state_render;
    game_intro_start_state.cleanup = state_cleanup;

    return &game_intro_start_state;
}