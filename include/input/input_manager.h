#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define MAX_PLAYERS 4
#define MAX_INPUTS 4

#include "input_mapping.h"

typedef struct InputManager
{
    InputMapping *inputs;        // array of input mappings for each player 0-3
    bool pad_active[MAX_INPUTS]; // true if a pad is connected 0-3 for players 0-3 (4 total)
    bool input_mapped[MAX_INPUTS];

    bool player_mapped[MAX_PLAYERS]; // true if a player is mapped to an input
    int player[MAX_PLAYERS];         // player input mapping to the correct input index, so player 2 can map to input 3 for example.

    bool (*axis_debounce)(const int input_index, const GamepadAxis axis, const float threshold);
    bool (*button_debounce)(const int input_index, const GamepadButton button);
    bool (*any_button_debounce)(void);
    bool (*key_debounce)(const int input_index, const KeyboardKey key);
    bool (*key_down_repeat)(const int input_index, const KeyboardKey key);

    void (*reset_player_inputs)(void);
    void (*map_player_input)(const int player, const int input_index); // map the player to the correct input index
    void (*unmap_player_input)(const int player);                // remove the player from the player mapping and shift down the other players
    InputMapping *(*get_player_input)(const int player);         // return the correct input mapping set
    void (*update)(const float delta_time);                      // check for active controllers and set pad_active whether a pad is connected or disconnected and set notification timeout
    void (*render)(void);                                  // render a notification if a pad becomes active or disconnects
    bool (*check_for_new_players)(const int player_count);       // check for new inputs and return true if a new input was pressed
    int (*input_pressed)(void);                            // return the index of the inputs array that was pressed, -1 if no input was pressed, useful for checking if a player has pressed a button
} InputManager;

InputManager *create_input_manager(void);
InputManager *get_input_manager(void);

#endif