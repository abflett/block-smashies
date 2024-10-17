#include "playing_input_handler.h"
#include "ship.h"
#include "input_manager.h"
#include "settings.h"
#include "game_state_manager.h"

#define MAX_SHIPS 4

static PlayingInputHandler playing_input;
static float boost_timer_left[MAX_SHIPS] = {0.0f};
static float boost_timer_right[MAX_SHIPS] = {0.0f};
static InputManager *input;
static InputMapping *p_inputs[MAX_SHIPS];
static int input_nums[MAX_SHIPS];
static Ship **ships;

static void update(float delta_time)
{
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

        // X movement
        if (IsKeyDown(p_inputs[i]->action_k_LEFT) ||                                //
            IsGamepadButtonDown(input_nums[i], p_inputs[i]->action_LEFT) ||         //
            GetGamepadAxisMovement(input_nums[i], p_inputs[i]->action_a_X) < -0.2f) //
        {
            ships[i]->move_ship(ships[i], -1);
        }
        if (IsKeyDown(p_inputs[i]->action_k_RIGHT) ||                              //
            IsGamepadButtonDown(input_nums[i], p_inputs[i]->action_RIGHT) ||       //
            GetGamepadAxisMovement(input_nums[i], p_inputs[i]->action_a_X) > 0.2f) //
        {
            ships[i]->move_ship(ships[i], 1);
        }

        // Boost logic for double tap
        if (IsKeyReleased(p_inputs[i]->action_k_LEFT))
        {
            boost_timer_left[i] = settings.gameplay.boost_timer;
        }
        if (IsKeyReleased(p_inputs[i]->action_k_RIGHT))
        {
            boost_timer_right[i] = settings.gameplay.boost_timer;
        }

        if ((IsKeyDown(p_inputs[i]->action_k_LEFT) && boost_timer_left[i] > 0.0f) || IsGamepadButtonPressed(input_nums[i], p_inputs[i]->action_L))
        {
            ships[i]->boost_ship(ships[i], -1);
        }
        if ((IsKeyDown(p_inputs[i]->action_k_RIGHT) && boost_timer_right[i] > 0.0f) || IsGamepadButtonPressed(input_nums[i], p_inputs[i]->action_R))
        {
            ships[i]->boost_ship(ships[i], 1);
        }

        if (IsKeyDown(p_inputs[i]->action_k_UP) || IsGamepadButtonPressed(input_nums[i], p_inputs[i]->action_A))
        {
            ships[i]->pulse_ship(ships[i]);
        }

        if (IsKeyPressed(p_inputs[i]->action_k_ESCAPE) || IsGamepadButtonPressed(input_nums[i], p_inputs[i]->action_START))
        {
            game_state_manager.context->game_status->is_pause = true;
            game_state_manager.change(game_state_manager.states.pause_menu);
            return;
        }
    }
}

PlayingInputHandler *initialize_playing_input(GameContext *context)
{
    input = get_input_manager();
    ships = context->entities->ships;
    for (int i = 0; i < context->game_data->player_count; i++)
    {
        p_inputs[i] = input->get_player_input(i);
        input_nums[i] = input->player[i];
    }

    playing_input.context = context;
    playing_input.update = update;

    return &playing_input;
}