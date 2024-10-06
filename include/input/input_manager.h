#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "input_mapping.h"

typedef struct InputManager
{
    InputMapping *inputs;
    int player_input[4]; // player to input index mapping

    InputMapping *(*get_player_input)(int player);           // get the correct mapping for selected player
    void (*set_player_mapping)(int player, int input_index); // setup player to selected input index

    void (*change_state)(void);       // change input state(main menu, embark, playing), pass in arguments so input can work on the context
    void (*update)(float delta_time); // check and take actions based on input
} InputManager;

InputManager *create_input_manager(void);

#endif