
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "virtual_keyboard.h"
#include "resource_manager.h"
#include "settings.h"

#define KEYBOARD_ROWS 4
#define KEYBOARD_COLS 10
#define KEYBOARD_KEYS "1234567890QWERTYUIOPASDFGHJKLZXCVBNM    "

static void keyboard_update(VirtualKeyboard *keyboard, float delta_time)
{
    if (!keyboard->active)
        return;

    // Example gamepad movement: move cursor with d-pad
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
        keyboard->selected_key_x--;
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
        keyboard->selected_key_x++;
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
        keyboard->selected_key_y--;
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
        keyboard->selected_key_y++;

    // Handle wrapping of cursor around the keyboard layout
    if (keyboard->selected_key_x < 0)
        keyboard->selected_key_x = KEYBOARD_COLS - 1;
    if (keyboard->selected_key_x >= KEYBOARD_COLS)
        keyboard->selected_key_x = 0;
    if (keyboard->selected_key_y < 0)
        keyboard->selected_key_y = KEYBOARD_ROWS - 1;
    if (keyboard->selected_key_y >= KEYBOARD_ROWS)
        keyboard->selected_key_y = 0;

    // Handle character input when pressing the "confirm" button (A or Enter)
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyPressed(KEY_ENTER))
    {
        int key_index = keyboard->selected_key_y * KEYBOARD_COLS + keyboard->selected_key_x;
        char selected_char = KEYBOARD_KEYS[key_index];

        if (keyboard->cursor_position < keyboard->max_length - 1)
        {
            keyboard->input_text[keyboard->cursor_position++] = selected_char;
            keyboard->input_text[keyboard->cursor_position] = '\0'; // Null-terminate the string
        }
    }

    // Handle backspace input
    if (IsKeyPressed(KEY_BACKSPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
    {
        if (keyboard->cursor_position > 0)
        {
            keyboard->input_text[--keyboard->cursor_position] = '\0';
        }
    }
}

static void keyboard_render(VirtualKeyboard *keyboard)
{
    DrawTextEx(*keyboard->font, "1234567890-=\nQWERTYUIOP[]\\\nASDFGHJKL;'\nZXCVBNM,./\n\n!@#$%^&*()_+\nqwertyuiop[]|\nasdfghjkl;'\nzxcvbnm,./\nSpace, Enter, Shift, Caps", (Vector2){1, 1}, 7, 0.0f, WHITE);

    if (!keyboard->active)
        return;

    DrawRectangle(0, (int)(settings.game.target_size.y * 0.5), (int)settings.game.target_size.x, (int)(settings.game.target_size.y * 0.5), settings.colors.black_alpha_05);

    // Render the input text at the specified position
    DrawTextEx(*keyboard->font, keyboard->input_text, keyboard->position, 7, 0.0f, WHITE);

    // Render the virtual keyboard (lower half of the screen)
    int key_width = 20;                // Adjusted key width for low-res
    int key_height = 20;               // Adjusted key height for low-res
    Vector2 keyboard_start = {50, 92}; // Start position for the keyboard on the screen

    for (int y = 0; y < KEYBOARD_ROWS; y++)
    {
        for (int x = 0; x < KEYBOARD_COLS; x++)
        {
            int key_index = y * KEYBOARD_COLS + x;
            char key = KEYBOARD_KEYS[key_index];
            Color key_color = (keyboard->selected_key_x == x && keyboard->selected_key_y == y) ? settings.colors.alpha_09 : settings.colors.alpha_05;

            Vector2 key_position = {keyboard_start.x + x * (key_width + 1), keyboard_start.y + y * (key_height + 1)};
            DrawRectangle((int)key_position.x + (y * 6), (int)key_position.y, key_width, key_height, key_color);

            // Center the text in the key
            Vector2 text_size = MeasureTextEx(*keyboard->font, TextFormat("%c", key), 7, 0.0f);
            Vector2 text_position = {(float)(int)(key_position.x + (key_width - text_size.x) / 2) + (y * 6), (float)(int)(key_position.y + (key_height - text_size.y) - 11)};
            DrawTextEx(*keyboard->font, TextFormat("%c", key), text_position, 7, 0.0f, BLACK);
        }
    }
}

static char *keyboard_get_string(VirtualKeyboard *keyboard)
{
    return keyboard->input_text;
}

static void keyboard_cleanup(VirtualKeyboard *keyboard)
{
    free(keyboard->input_text);
    free(keyboard);
}

VirtualKeyboard *create_virtual_keyboard(Vector2 position, int max_length)
{
    VirtualKeyboard *keyboard = (VirtualKeyboard *)malloc(sizeof(VirtualKeyboard));

    keyboard->font = resource_manager.get_pixel7_font();

    keyboard->input_text = (char *)calloc(max_length + 1, sizeof(char)); // Allocate memory for the string
    keyboard->max_length = max_length;
    keyboard->cursor_position = 0;
    keyboard->position = position;
    keyboard->selected_key_x = 0;
    keyboard->selected_key_y = 0;
    keyboard->active = true;

    keyboard->update = keyboard_update;
    keyboard->render = keyboard_render;
    keyboard->get_string = keyboard_get_string;
    keyboard->cleanup = keyboard_cleanup;

    return keyboard;
}
