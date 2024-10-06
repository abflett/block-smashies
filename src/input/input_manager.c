#include "input_manager.h"
#include "settings.h"

static InputManager manager;

// Function to retrieve player input mapping
InputMapping *get_player_input(int player)
{
    if (player < 0 || player >= 4)
    {
        return NULL; // Handle invalid player index
    }
    return &manager.inputs[manager.player_input[player]]; // Return the corresponding input mapping
}

// Function to set the player mapping to a new input index
void set_player_mapping(int player, int input_index)
{
    if (player < 0 || player >= 4 || input_index < 0 || input_index >= 4)
    {
        return; // Handle invalid indices
    }
    manager.player_input[player] = input_index; // Update the mapping
}

InputManager *create_input_manager(void)
{
    manager.inputs = settings.inputs;

    for (int i = 0; i < 4; i++)
    {
        manager.player_input[i] = i; // Initialize to default to player mapping index
    }

    manager.get_player_input = get_player_input; // Assign function pointers
    manager.set_player_mapping = set_player_mapping;
    // manager.change_state = change_state; // Define this function
    // manager.update = update; // Define this function

    return &manager;
}