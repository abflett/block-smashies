#include "playing_input_handler.h"
#include "ship.h"
#include "game.h"

#define MAX_SHIPS 4

PlayingInputHandler playing_input;

static void update(void)
{
    InputManager *input = get_input_manager();
    for (int i = 0; i < playing_input.context->game_data->player_count; i++)
    {
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
    }
}

PlayingInputHandler *initialize_playing_input(GameContext *context)
{
    playing_input.context = context;

    playing_input.update = update;
    return &playing_input;
}