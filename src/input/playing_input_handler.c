#include "playing_input_handler.h"
#include "ship.h"
#include "game.h"
#include "settings.h"

#define MAX_SHIPS 4

static PlayingInputHandler playing_input;
static float boost_timer_left[MAX_SHIPS] = {0.0f};
static float boost_timer_right[MAX_SHIPS] = {0.0f};

static void update(float delta_time)
{
    InputManager *input = get_input_manager();
    for (int i = 0; i < playing_input.context->game_data->player_count; i++)
    {
        if (boost_timer_left[i] > 0.0f)
        {
            boost_timer_left[i] -= delta_time;
        }
        if (boost_timer_right[i] > 0.0f)
        {
            boost_timer_right[i] -= delta_time;
        }

        InputMapping *p_input = input->get_player_input(i);
        Ship *ship = playing_input.context->entities->ships[i];

        if (IsKeyDown(p_input->action_k_LEFT))
        {
            ship->move_left(ship);
        }
        if (IsKeyDown(p_input->action_k_RIGHT))
        {
            ship->move_right(ship);
        }

        if (IsKeyReleased(p_input->action_k_LEFT))
        {
            boost_timer_left[i] = settings.gameplay.boost_timer;
        }
        if (IsKeyReleased(p_input->action_k_RIGHT))
        {
            boost_timer_right[i] = settings.gameplay.boost_timer;
        }

        if (IsKeyDown(p_input->action_k_LEFT) && boost_timer_left[i] > 0.0f)
        {
            ship->boost_left(ship);
        }
        if (IsKeyDown(p_input->action_k_RIGHT) && boost_timer_right[i] > 0.0f)
        {
            ship->boost_right(ship);
        }

        // boost logic
    }
}

PlayingInputHandler *initialize_playing_input(GameContext *context)
{
    playing_input.context = context;

    playing_input.update = update;
    return &playing_input;
}