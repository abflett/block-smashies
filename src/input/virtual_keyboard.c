
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "raylib.h"
#include "virtual_keyboard.h"
#include "resource_manager.h"
#include "settings.h"
#include "game.h"

#define KEYBOARD_ROWS 5
#define KEYBOARD_COLS 12
#define KEYBOARD_KEYS "*1234567890<*qwertyuiop*+asdfghjkl']=zxcvbnm,._*>>>>>>>>>>>>"
// *1234567890< // (*)none, (<)delete // row 0
// *QWERTYUIOP* // (*)none, (*)none   // row 1
// +ASDFGHJKL'] // (+)caps, (])enter  // row 2
// =ZXCVBNM,._* // (=)shift, (*)none  // row 3
// >>>>>>>>>>>> // (>)space...        // row 4

static void keyboard_activate(VirtualKeyboard *keyboard)
{
    keyboard->active = true;
    keyboard->is_closing = false;
    keyboard->keyboard_position = (Vector2){keyboard->keyboard_position.x, settings.game.target_size.y - 20};
    int input_length = (int)strlen(keyboard->input_text);
    keyboard->cursor_position = input_length;
}

static void keyboard_update(VirtualKeyboard *keyboard, float delta_time)
{
    if (!keyboard->active && !keyboard->is_closing)
    {
        return;
    }

    if (keyboard->is_closing)
    {
        if (keyboard->keyboard_position.y < settings.game.target_size.y)
        {
            keyboard->keyboard_position.y += delta_time * 200;
        }
        else
        {
            keyboard->is_closing = false;
            keyboard->active = false;
        }
    }
    else
    {
        if (keyboard->keyboard_position.y > keyboard->keyboard_final_position.y)
        {
            keyboard->keyboard_position.y -= delta_time * 200;
        }
        else
        {
            keyboard->keyboard_position = keyboard->keyboard_final_position;
        }

        // move back down if enter was pressed.

        // close virtural keyboard if enter key is pressed
        if (keyboard->input_manager->key_debounce(0, keyboard->p1_input->action_k_ENTER))
        {
            keyboard->is_closing = true;
        }

        keyboard->blink_time -= delta_time;

        if (keyboard->blink_time < 0)
        {
            keyboard->blink_time = 0.25f;
            keyboard->show_underscore = !keyboard->show_underscore;
        }

        // Gamepad movement: move cursor with d-pad
        bool moved = false;

        // Store previous positions
        int previous_x = 0;
        int previous_y = 0;

        // Movement input handling
        if (IsGamepadButtonPressed(0, keyboard->p1_input->action_LEFT) || keyboard->input_manager->axis_debounce(0, keyboard->p1_input->action_a_X, -0.5f))
        {
            keyboard->selected_key_x--;
            previous_x = -1;
            moved = true;
        }
        if (IsGamepadButtonPressed(0, keyboard->p1_input->action_RIGHT) || keyboard->input_manager->axis_debounce(0, keyboard->p1_input->action_a_X, 0.5f))
        {
            keyboard->selected_key_x++;
            previous_x = 1;
            moved = true;
        }
        if (IsGamepadButtonPressed(0, keyboard->p1_input->action_UP) || keyboard->input_manager->axis_debounce(0, keyboard->p1_input->action_a_Y, -0.5f))
        {
            keyboard->selected_key_y--;
            previous_y = -1;
            moved = true;
        }
        if (IsGamepadButtonPressed(0, keyboard->p1_input->action_DOWN) || keyboard->input_manager->axis_debounce(0, keyboard->p1_input->action_a_Y, 0.5f))
        {
            keyboard->selected_key_y++;
            previous_y = 1;
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

            // If currently on '*', move back to the previous position
            while (selected_char == '*')
            {
                if (previous_y != 0)
                {
                    keyboard->selected_key_x += keyboard->selected_key_x < 6 ? 1 : -1;
                }

                keyboard->selected_key_x += previous_x;

                // Wrap around the keyboard layout
                if (keyboard->selected_key_x < 0)
                    keyboard->selected_key_x = KEYBOARD_COLS - 1;
                if (keyboard->selected_key_x >= KEYBOARD_COLS)
                    keyboard->selected_key_x = 0;
                if (keyboard->selected_key_y < 0)
                    keyboard->selected_key_y = KEYBOARD_ROWS - 1;
                if (keyboard->selected_key_y >= KEYBOARD_ROWS)
                    keyboard->selected_key_y = 0;

                // Update the key index and check again
                key_index = keyboard->selected_key_y * KEYBOARD_COLS + keyboard->selected_key_x;
                selected_char = KEYBOARD_KEYS[key_index];
            }
        }

        // Handle character input when pressing the "confirm" button (A or Enter)
        if (IsGamepadButtonPressed(0, keyboard->p1_input->action_A))
        {
            int key_index = keyboard->selected_key_y * KEYBOARD_COLS + keyboard->selected_key_x;
            char selected_char = KEYBOARD_KEYS[key_index];

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
                keyboard->is_closing = true;
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

        // Check if the Caps Lock key is pressed
        // Weird hack as if I only check for key pressed it will require 2 pressed
        if (IsKeyReleased(KEY_CAPS_LOCK) || IsKeyPressed(KEY_CAPS_LOCK))
        {
            keyboard->caps_on = !keyboard->caps_on; // Toggle caps state
        }

        int key = GetKeyPressed(); // Get the key pressed
        keyboard->shift_down = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

        // Check for printable characters
        if (key >= 32 && key <= 126)
        { // Printable ASCII range
            if (keyboard->cursor_position < keyboard->max_length)
            { // Ensure room for null terminator
                char selected_char = (char)key;
                selected_char = (keyboard->caps_on || keyboard->shift_on || keyboard->shift_down) ? selected_char : tolower(selected_char);
                keyboard->input_text[keyboard->cursor_position++] = selected_char;
                keyboard->input_text[keyboard->cursor_position] = '\0'; // Null-terminate the string
                keyboard->shift_on = false;

                // Map the typed character to the virtual keyboard's layout
                for (int y = 0; y < KEYBOARD_ROWS; y++)
                {
                    for (int x = 0; x < KEYBOARD_COLS; x++)
                    {
                        int key_index = y * KEYBOARD_COLS + x;
                        if (KEYBOARD_KEYS[key_index] == tolower(selected_char))
                        {
                            // Set the selected key's position
                            keyboard->selected_key_x = x;
                            keyboard->selected_key_y = y;
                            break;
                        }
                    }
                }
            }
        }

        // Handle backspace input // Todo: inputmanager keydebounce
        if (keyboard->input_manager->key_debounce(0, KEY_BACKSPACE) || keyboard->input_manager->key_down_repeat(0, KEY_BACKSPACE) || IsGamepadButtonPressed(0, keyboard->p1_input->action_B))
        {
            if (keyboard->cursor_position > 0)
            {
                keyboard->selected_key_x = 11;
                keyboard->selected_key_y = 0;
                keyboard->input_text[--keyboard->cursor_position] = '\0';
            }
        }
    }
}

static void keyboard_render(VirtualKeyboard *keyboard)
{
    if (!keyboard->active && !keyboard->is_closing)
        return;

    // If we haven't reached max_length, show the blinking underscore at the typing position
    if (keyboard->cursor_position < keyboard->max_length && keyboard->show_underscore)
    {
        Vector2 text_size = MeasureTextEx(*keyboard->font, keyboard->input_text, 7, 0.0f);
        Vector2 underscore_position = {
            keyboard->text_position.x + text_size.x / 2, // At the end of the input text
            keyboard->text_position.y};
        DrawTextEx(*keyboard->font, "_", underscore_position, 7, 0.0f, keyboard->blink_color); // Draw the blinking underscore
    }

    // Render the virtual keyboard (lower half of the screen)
    // Keyboard background
    DrawTexture(*keyboard->keyboard_bg, (int)keyboard->keyboard_position.x, (int)keyboard->keyboard_position.y, WHITE);

    int key_width = 15;
    int key_height = 15;
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

            key = keyboard->caps_on || keyboard->shift_on || keyboard->shift_down ? toupper(key) : key;

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

    if (keyboard->shift_on || keyboard->shift_down)
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
    free(keyboard);
}

VirtualKeyboard *create_virtual_keyboard(char *input_text, int max_length, Vector2 text_position, Vector2 keyboard_position, Color blink_color)
{
    VirtualKeyboard *keyboard = (VirtualKeyboard *)malloc(sizeof(VirtualKeyboard));

    keyboard->font = resource_manager.get_pixel7_font();
    keyboard->input_manager = get_input_manager();
    keyboard->p1_input = keyboard->input_manager->get_player_input(0);

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

    keyboard->blink_color = blink_color;
    keyboard->caps_on = false;
    keyboard->shift_on = false;
    keyboard->input_text = input_text; // Allocate memory for the string
    keyboard->max_length = max_length;
    keyboard->cursor_position = 0;
    keyboard->text_position = text_position;
    keyboard->keyboard_position = (Vector2){keyboard_position.x, settings.game.target_size.y};
    keyboard->keyboard_final_position = keyboard_position;
    keyboard->selected_key_x = 1;
    keyboard->selected_key_y = 0;
    keyboard->active = false;
    keyboard->show_underscore = true;
    keyboard->is_closing = false;

    keyboard->update = keyboard_update;
    keyboard->render = keyboard_render;
    keyboard->activate = keyboard_activate;
    keyboard->get_string = keyboard_get_string;
    keyboard->cleanup = keyboard_cleanup;

    return keyboard;
}