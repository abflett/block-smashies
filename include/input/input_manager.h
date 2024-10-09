#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define MAX_PLAYERS 4
#define MAX_INPUTS 4

#include "input_mapping.h"

typedef struct InputManager
{
    InputMapping *inputs;        // array of input mappings for each player 0-3
    bool pad_active[MAX_INPUTS]; // true if a pad is connected 0-3 for players 0-3 (4 total)

    void (*update)(float delta_time); // check for active controllers and set pad_active whether a pad is connected or disconnected and set notification timeout
    void (*render)(void);             // render a notification if a pad becomes active or disconnects
    int (*input_pressed)(void);       // return the index of the inputs array that was pressed, -1 if no input was pressed, useful for checking if a player has pressed a button
} InputManager;

InputManager *create_input_manager(void);

#endif