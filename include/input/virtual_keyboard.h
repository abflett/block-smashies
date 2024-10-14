#ifndef VIRTUAL_KEYBOARD_H
#define VIRTUAL_KEYBOARD_H

#include <stdbool.h>
#include "raylib.h"
#include "input_manager.h"

typedef struct VirtualKeyboard
{
    char *input_text; // Stores the input string
    Font *font;       // Font to use for the keyboard
    Color blink_color;
    InputMapping *p1_input;
    InputManager *input_manager;

    Texture2D *keyboard_bg;         // Texture for the keyboard background
    Texture2D *keyboard_key_lit;    // Texture for the keyboard key when it is pressed
    Texture2D *keyboard_key;        // Texture for the keyboard key
    Texture2D *keyboard_caps_lit;   // Texture for the keyboard key when it is pressed
    Texture2D *keyboard_caps;       // Texture for the keyboard key
    Texture2D *keyboard_delete_lit; // Texture for the keyboard key when it is pressed
    Texture2D *keyboard_delete;     // Texture for the keyboard key
    Texture2D *keyboard_enter_lit;  // Texture for the keyboard key when it is pressed
    Texture2D *keyboard_enter;      // Texture for the keyboard key
    Texture2D *keyboard_shift_lit;  // Texture for the keyboard key when it is pressed
    Texture2D *keyboard_shift;      // Texture for the keyboard key
    Texture2D *keyboard_space_lit;  // Texture for the keyboard key when it is pressed
    Texture2D *keyboard_space;      // Texture for the keyboard key
    bool shift_on;
    bool shift_down;
    bool caps_on;
    float blink_time;
    bool show_underscore;
    int max_length;                     // Max length of the input string
    int cursor_position;                // Position of the cursor for text editing
    int selected_key_x, selected_key_y; // Current position on the virtual keyboard
    Vector2 text_position;              // Position of where the text will be displayed
    Vector2 keyboard_position;
    bool active;                                                        // True if the virtual keyboard is in use
    void (*update)(struct VirtualKeyboard *keyboard, float delta_time); // Update logic for input
    void (*render)(struct VirtualKeyboard *keyboard);                   // Render the keyboard
    char *(*get_string)(struct VirtualKeyboard *keyboard);              // Return the current string
    void (*cleanup)(struct VirtualKeyboard *keyboard);                  // Cleanup memory, etc.
} VirtualKeyboard;

VirtualKeyboard *create_virtual_keyboard(char *input_text, int max_length, Vector2 text_position, Vector2 keyboard_position, Color blink_color);
#endif
