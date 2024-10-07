#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define MAX_PLAYERS 4
#define MAX_INPUTS 4

#include "input_mapping.h"

typedef struct InputManager
{
    int input_count;
} InputManager;

InputManager *create_input_manager(void);

#endif