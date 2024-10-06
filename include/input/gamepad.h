#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <stdbool.h>

typedef struct Gamepad
{
    char *name;
    bool active;
    int index;

    bool buttons[17]; // 17 possible buttons (see GamepadButton enum)
    float axes[6];    // 6 possible axes (see GamepadAxis enum)
} Gamepad;

#endif