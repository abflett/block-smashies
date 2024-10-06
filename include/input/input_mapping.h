#ifndef INPUT_MAPPING_H
#define INPUT_MAPPING_H

#include "raylib.h"

typedef struct InputMapping
{                                // Menus (most)    | Embark/Ship     | Gameplay
                                 // ================|=================|=============
    GamepadButton action_UP;     // Cycle Backwards | Color Backwards | None
    GamepadButton action_RIGHT;  // Cycle Forwards  | Cycle Forwards  | Acc Right
    GamepadButton action_DOWN;   // Cycle Forwards  | Color Forwards  | None
    GamepadButton action_LEFT;   // Cycle Backwards | Cycle Backwards | Acc Left
    GamepadButton action_B;      // Exit / Back     | Exit / Back     | Fire Bullet
    GamepadButton action_A;      // Enter / Select  | Enter / Select  | Pulse
    GamepadButton action_L;      // None            | None            | Boost Left
    GamepadButton action_R;      // None            | None            | Boost Right
    GamepadButton action_START;  // Enter / Select  | Enter / Select  | Pause Menu
                                 // ================|=================|=============
    KeyboardKey action_k_UP;     // Cycle Backwards | Color Backwards | Pulse
    KeyboardKey action_k_RIGHT;  // Cycle Forwards  | Color Forwards  | Acc Right
    KeyboardKey action_k_DOWN;   // Cycle Forwards  | Color Forwards  | Fire Bullet
    KeyboardKey action_k_LEFT;   // Cycle Backwards | Color Backwards | Acc Left
    KeyboardKey action_k_ENTER;  // Enter / Select  | Enter / Select  | None
    KeyboardKey action_k_ESCAPE; // Exit / Back     | Exit / Back     | Pause Menu
                                 // ================|=================|=============
    GamepadAxis action_a_x;      // Cycle +/-       | Cycle +/-       | Acc +/-
    GamepadAxis action_a_y;      // Cycle +/-       | Color +/-       | None
} InputMapping;

#endif