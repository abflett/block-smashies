
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "raylib.h"
#include "virtual_keyboard.h"
#include "resource_manager.h"
#include "settings.h"

#define KEYBOARD_ROWS 5
#define KEYBOARD_COLS 12
#define KEYBOARD_KEYS "*1234567890<*qwertyuiop*+asdfghjkl']=zxcvbnm,._*>>>>>>>>>>>>"

static void keyboard_update(VirtualKeyboard *keyboard, float delta_time)
{
    if (!keyboard->active)
        return;

    // Gamepad movement: move cursor with d-pad
    bool moved = false;
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
    {
        keyboard->selected_key_x--;
        moved = true;
    }
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
    {
        keyboard->selected_key_x++;
        moved = true;
    }
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
    {
        keyboard->selected_key_y--;
        moved = true;
    }
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
    {
        keyboard->selected_key_y++;
        moved = true;
    }

    // Handle wrapping of cursor around the keyboard layout
    if (keyboard->selected_key_x < 0)
        keyboard->selected_key_x = KEYBOARD_COLS - 1;
    if (keyboard->selected_key_x >= KEYBOARD_COLS)
        keyboard->selected_key_x = 0;
    if (keyboard->selected_key_y < 0)
        keyboard->selected_key_y = KEYBOARD_ROWS - 1;
    if (keyboard->selected_key_y >= KEYBOARD_ROWS)
        keyboard->selected_key_y = 0;

    // Adjust selection to avoid '*' and handle wrapping after movement
    if (moved)
    {
        int key_index = keyboard->selected_key_y * KEYBOARD_COLS + keyboard->selected_key_x;
        char selected_char = KEYBOARD_KEYS[key_index];

        while (selected_char == '*')
        {
            if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
            {
                keyboard->selected_key_x--;
                if (keyboard->selected_key_x < 0)
                    keyboard->selected_key_x = KEYBOARD_COLS - 1;
            }
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
            {
                keyboard->selected_key_x++;
                if (keyboard->selected_key_x >= KEYBOARD_COLS)
                    keyboard->selected_key_x = 0;
            }
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
            {
                keyboard->selected_key_y--;
                if (keyboard->selected_key_y < 0)
                    keyboard->selected_key_y = KEYBOARD_ROWS - 1;
            }
            else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
            {
                keyboard->selected_key_y++;
                if (keyboard->selected_key_y >= KEYBOARD_ROWS)
                    keyboard->selected_key_y = 0;
            }

            // Update key index and check again
            key_index = keyboard->selected_key_y * KEYBOARD_COLS + keyboard->selected_key_x;
            selected_char = KEYBOARD_KEYS[key_index];
        }
    }

    // Handle character input when pressing the "confirm" button (A or Enter)
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyPressed(KEY_ENTER))
    {
        int key_index = keyboard->selected_key_y * KEYBOARD_COLS + keyboard->selected_key_x;
        char selected_char = KEYBOARD_KEYS[key_index];

        // *1234567890< // none, delete
        // *QWERTYUIOP* // none, none
        // +ASDFGHJKL'] // caps, enter
        // =ZXCVBNM,._* // shift, none
        // >>>>>>>>>>>> // space-space

        switch (selected_char)
        {
        case '<':
            if (keyboard->cursor_position > 0)
            {
                keyboard->input_text[--keyboard->cursor_position] = '\0';
            }
            break;
        case '+':
            keyboard->caps_on = !keyboard->caps_on;
            break;
        case ']':
            keyboard->active = false;
            break;
        case '=':
            keyboard->shift_on = !keyboard->shift_on;
            break;
        case '>':
            if (keyboard->cursor_position < keyboard->max_length)
            {
                keyboard->input_text[keyboard->cursor_position++] = ' ';
                keyboard->input_text[keyboard->cursor_position] = '\0'; // Null-terminate the string
                break;
            }
            break;

        default:
            if (keyboard->cursor_position < keyboard->max_length)
            {
                keyboard->input_text[keyboard->cursor_position++] = keyboard->caps_on || keyboard->shift_on ? toupper(selected_char) : selected_char;
                keyboard->input_text[keyboard->cursor_position] = '\0'; // Null-terminate the string
                keyboard->shift_on = false;
                break;
            }
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
    if (!keyboard->active)
        return;

    DrawTexture(*keyboard->keyboard_bg, (int)keyboard->keyboard_position.x, (int)keyboard->keyboard_position.y, WHITE);

    // Render the input text at the specified position
    DrawTextEx(*keyboard->font, keyboard->input_text, keyboard->text_position, 7, 0.0f, keyboard->font_color);

    // Render the virtual keyboard (lower half of the screen)
    int key_width = 15;                                                                               // Adjusted key width for low-res
    int key_height = 15;                                                                              // Adjusted key height for low-res
    Vector2 keyboard_start = {keyboard->keyboard_position.x + 23, keyboard->keyboard_position.y + 6}; // Start position for the keyboard on the screen
    int key_offsets_x[KEYBOARD_ROWS] = {-15, -5, 0, 10, 0};

    Texture2D *shift_texture = keyboard->keyboard_shift;
    Texture2D *caps_texture = keyboard->keyboard_caps;
    Texture2D *space_texture = keyboard->keyboard_space;
    Texture2D *enter_texture = keyboard->keyboard_enter;
    Texture2D *delete_texture = keyboard->keyboard_delete;

    for (int y = 0; y < KEYBOARD_ROWS; y++)
    {
        for (int x = 0; x < KEYBOARD_COLS; x++)
        {
            int key_index = y * KEYBOARD_COLS + x;
            char key = KEYBOARD_KEYS[key_index];
            bool is_selected = keyboard->selected_key_x == x && keyboard->selected_key_y == y;

            if (key == '*' || key == '<' || key == '+' || key == ']' || key == '=' || key == '>')
            {
                if (is_selected)
                {
                    switch (key)
                    {
                    case '<':
                        delete_texture = keyboard->keyboard_delete_lit;
                        break;
                    case '+':
                        caps_texture = keyboard->keyboard_caps_lit;
                        break;
                    case ']':
                        enter_texture = keyboard->keyboard_enter_lit;
                        break;
                    case '=':
                        shift_texture = keyboard->keyboard_shift_lit;
                        break;
                    case '>':
                        space_texture = keyboard->keyboard_space_lit;
                        break;
                    default:
                        break;
                    }
                }

                continue;
            }

            key = keyboard->caps_on || keyboard->shift_on ? toupper(key) : key;

            Vector2 key_position = {keyboard_start.x + x * (key_width + 1), keyboard_start.y + y * (key_height + 1)};
            Texture2D *key_texture = is_selected ? keyboard->keyboard_key_lit : keyboard->keyboard_key;
            DrawTexture(*key_texture, (int)key_position.x + key_offsets_x[y], (int)key_position.y, WHITE);

            // Center the text in the key
            Vector2 text_size = MeasureTextEx(*keyboard->font, TextFormat("%c", key), 7, 0.0f);
            Vector2 text_position = {(float)(int)(key_position.x + (key_width - text_size.x) / 2) + key_offsets_x[y] + 1, (float)(int)(key_position.y + (key_height - text_size.y) / 2)};
            Color key_text_color = is_selected ? settings.colors.blue_05 : settings.colors.blue_03;
            DrawTextEx(*keyboard->font, TextFormat("%c", key), text_position, 7, 0.0f, key_text_color);
        }
    }

    if (keyboard->caps_on)
    {
        keyboard->shift_on = false;
        caps_texture = keyboard->keyboard_caps_lit;
    }

    if (keyboard->shift_on)
    {
        shift_texture = keyboard->keyboard_shift_lit;
        keyboard->caps_on = false;
    }

    DrawTexture(*space_texture, (int)keyboard_start.x + 50, (int)keyboard_start.y + 64, WHITE);
    DrawTexture(*caps_texture, (int)keyboard_start.x - 14, (int)keyboard_start.y + 32, WHITE);
    DrawTexture(*enter_texture, (int)keyboard_start.x + 176, (int)keyboard_start.y + 32, WHITE);
    DrawTexture(*delete_texture, (int)keyboard_start.x + 161, (int)keyboard_start.y + 0, WHITE);
    DrawTexture(*shift_texture, (int)keyboard_start.x - 5, (int)keyboard_start.y + 48, WHITE);
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

VirtualKeyboard *create_virtual_keyboard(Vector2 text_position, Vector2 keyboard_position, int max_length, Color font_color)
{
    VirtualKeyboard *keyboard = (VirtualKeyboard *)malloc(sizeof(VirtualKeyboard));

    keyboard->font = resource_manager.get_pixel7_font();

    keyboard->keyboard_bg = &resource_manager.get_texture("keyboard-bg")->texture;
    keyboard->keyboard_key_lit = &resource_manager.get_texture("keyboard-key-lit")->texture;
    keyboard->keyboard_key = &resource_manager.get_texture("keyboard-key")->texture;
    keyboard->keyboard_caps_lit = &resource_manager.get_texture("keyboard-caps-lit")->texture;
    keyboard->keyboard_caps = &resource_manager.get_texture("keyboard-caps")->texture;
    keyboard->keyboard_delete_lit = &resource_manager.get_texture("keyboard-delete-lit")->texture;
    keyboard->keyboard_delete = &resource_manager.get_texture("keyboard-delete")->texture;
    keyboard->keyboard_enter_lit = &resource_manager.get_texture("keyboard-enter-lit")->texture;
    keyboard->keyboard_enter = &resource_manager.get_texture("keyboard-enter")->texture;
    keyboard->keyboard_shift_lit = &resource_manager.get_texture("keyboard-shift-lit")->texture;
    keyboard->keyboard_shift = &resource_manager.get_texture("keyboard-shift")->texture;
    keyboard->keyboard_space_lit = &resource_manager.get_texture("keyboard-space-lit")->texture;
    keyboard->keyboard_space = &resource_manager.get_texture("keyboard-space")->texture;

    keyboard->font_color = font_color;
    keyboard->caps_on = false;
    keyboard->shift_on = false;
    keyboard->input_text = (char *)calloc(max_length + 1, sizeof(char)); // Allocate memory for the string
    keyboard->max_length = max_length;
    keyboard->cursor_position = 0;
    keyboard->text_position = text_position;
    keyboard->keyboard_position = keyboard_position;
    keyboard->selected_key_x = 1;
    keyboard->selected_key_y = 0;
    keyboard->active = true;

    keyboard->update = keyboard_update;
    keyboard->render = keyboard_render;
    keyboard->get_string = keyboard_get_string;
    keyboard->cleanup = keyboard_cleanup;

    return keyboard;
}