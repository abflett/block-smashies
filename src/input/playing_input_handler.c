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
        InputMapping *p_input = input->get_player_input(i);
        int input_num = input->player[i];

        Ship *ship = playing_input.context->entities->ships[i];

        if (boost_timer_left[i] > 0.0f)
        {
            boost_timer_left[i] -= delta_time;
        }
        if (boost_timer_right[i] > 0.0f)
        {
            boost_timer_right[i] -= delta_time;
        }

        // X movement
        if (IsKeyDown(p_input->action_k_LEFT) ||                            //
            IsGamepadButtonDown(input_num, p_input->action_LEFT) ||         //
            GetGamepadAxisMovement(input_num, p_input->action_a_X) < -0.2f) //
        {
            ship->move_ship(ship, -1);
        }
        if (IsKeyDown(p_input->action_k_RIGHT) ||                          //
            IsGamepadButtonDown(input_num, p_input->action_RIGHT) ||       //
            GetGamepadAxisMovement(input_num, p_input->action_a_X) > 0.2f) //
        {
            ship->move_ship(ship, 1);
        }

        // Boost logic for double tap
        if (IsKeyReleased(p_input->action_k_LEFT))
        {
            boost_timer_left[i] = settings.gameplay.boost_timer;
        }
        if (IsKeyReleased(p_input->action_k_RIGHT))
        {
            boost_timer_right[i] = settings.gameplay.boost_timer;
        }

        if ((IsKeyDown(p_input->action_k_LEFT) && boost_timer_left[i] > 0.0f) || IsGamepadButtonPressed(input_num, p_input->action_L))
        {
            ship->boost_ship(ship, -1);
        }
        if ((IsKeyDown(p_input->action_k_RIGHT) && boost_timer_right[i] > 0.0f) || IsGamepadButtonPressed(input_num, p_input->action_R))
        {
            ship->boost_ship(ship, 1);
        }

        if (IsKeyDown(p_input->action_k_UP) || IsGamepadButtonPressed(input_num, p_input->action_A))
        {
            ship->pulse_ship(ship);
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