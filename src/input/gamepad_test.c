
#include <string.h>
#include "raylib.h"
#include "gamepad_test.h"
#include "resource_manager.h"
#include "gamepad.h"

static GamepadTest gamepad_test;

static Gamepad gamepads[4];

static Font *pixel7;

static void update_gamepad_test(const float delta_time)
{
    for (int i = 0; i < 4; i++)
    {
        if (IsGamepadAvailable(i))
        {
            gamepads[i].active = true;
            gamepads[i].index = i;

            const char *gamepadName = GetGamepadName(i);
            if (gamepadName && strcmp(gamepads[i].name, gamepadName) != 0)
            {
                free(gamepads[i].name);
                gamepads[i].name = malloc(strlen(gamepadName) + 1);
                strcpy(gamepads[i].name, gamepadName);
            }

            // Capture button states
            for (int button = GAMEPAD_BUTTON_LEFT_FACE_UP; button <= GAMEPAD_BUTTON_RIGHT_THUMB; button++)
            {
                if(button <= GAMEPAD_BUTTON_LEFT_THUMB)
                {
                    gamepads[i].buttons[button] = IsGamepadButtonDown(i, button);
                }

            }

            // Capture axis states
            const int axisCount = GetGamepadAxisCount(i);
            for (int axis = GAMEPAD_AXIS_LEFT_X; axis < axisCount; axis++)
            {
                gamepads[i].axes[axis] = GetGamepadAxisMovement(i, axis);
            }
        }
        else
        {
            if (gamepads[i].active)
            {
                gamepads[i].active = false;
                free(gamepads[i].name);
                gamepads[i].name = strdup("Unknown");
                gamepads[i].index = -1;
            }
        }
    }
}

static void render_gamepad_test(void)
{
    for (int i = 0; i < 4; i++)
    {
        const char *text = TextFormat("%s [%d]: (%s)", gamepads[i].name, gamepads[i].index, gamepads[i].active ? "active" : "inactive");
        DrawTextEx(*pixel7, text, (Vector2){0, (float)i * 45.0f}, 7, 0.0f, WHITE);

        char buttonText[256] = "Pressed: ";
        int position = 9; // Initialize position after "Pressed: "

        if (gamepads[i].active)
        {
            for (int button = GAMEPAD_BUTTON_LEFT_FACE_UP; button <= GAMEPAD_BUTTON_RIGHT_THUMB; button++)
            {
                if (button <= GAMEPAD_BUTTON_LEFT_THUMB && gamepads[i].buttons[button]) // Check if button is pressed
                {
                    const char *btn_name = TextFormat("%d, ", button); // Format button index;
                    TextAppend(buttonText, btn_name, &position);       // Append button index to buttonText
                }
            }

            // Remove the last comma and space if there's at least one pressed button
            if (position > TextLength("Pressed: ")) // Check if any button was pressed
            {
                buttonText[position - 2] = '\0'; // Remove the last ", "
            }

            DrawTextEx(*pixel7, buttonText, (Vector2){0, (float)i * 45.0f + 10.0f}, 7, 0.0f, WHITE);

            // Create axis state string
            const char *axis_text = TextFormat("AxisLX: %.2f, AxisLY: %.2f - AxisRX: %.2f, AxisRY: %.2f - TriggerL: %.2f, TriggerR: %.2f",
                                               gamepads[i].axes[GAMEPAD_AXIS_LEFT_X],
                                               gamepads[i].axes[GAMEPAD_AXIS_LEFT_Y],
                                               gamepads[i].axes[GAMEPAD_AXIS_RIGHT_X],
                                               gamepads[i].axes[GAMEPAD_AXIS_RIGHT_Y],
                                               gamepads[i].axes[GAMEPAD_AXIS_LEFT_TRIGGER],
                                               gamepads[i].axes[GAMEPAD_AXIS_RIGHT_TRIGGER]);

            DrawTextEx(*pixel7, axis_text, (Vector2){0, (float)i * 45.0f + 20.0f}, 7, 0.0f, WHITE);
        }
    }
}

static void cleanup_gamepad_test(void)
{
    for (int i = 0; i < 4; i++)
    {
        if (gamepads[i].name)
        {
            free(gamepads[i].name); // Free allocated name memory
        }
    }
}

GamepadTest *create_gamepad_test(void)
{
    pixel7 = resource_manager.get_pixel7_font();

    for (int i = 0; i < 4; i++)
    {
        if (IsGamepadAvailable(i))
        {
            const char *gamepadName = GetGamepadName(i);
            if (gamepadName)
            {
                gamepads[i].name = malloc(strlen(gamepadName) + 1);
                strcpy(gamepads[i].name, gamepadName);
            }
            else
            {
                gamepads[i].name = strdup("Unknown");
            }

            gamepads[i].active = true;
            gamepads[i].index = i;
        }
        else
        {
            gamepads[i].active = false;
            gamepads[i].name = strdup("Unknown");
            gamepads[i].index = -1;
        }
    }

    gamepad_test.update = update_gamepad_test;
    gamepad_test.render = render_gamepad_test;
    gamepad_test.cleanup = cleanup_gamepad_test;
    return &gamepad_test;
}