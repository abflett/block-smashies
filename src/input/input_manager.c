#include "raylib.h"
#include "input_manager.h"
#include "settings.h"
#include "resource_manager.h"
#include "math.h"

static InputManager manager;
static float notification_timeout[MAX_INPUTS];
static Font *font;

static void update(float delta_time)
{
    for (int i = 0; i < MAX_INPUTS; i++)
    {
        bool connected = IsGamepadAvailable(i);
        if (connected != manager.pad_active[i])
        {
            notification_timeout[i] = 4.0f;
            TraceLog(LOG_INFO, "Gamepad %d connected: %d", i, connected);
        }
        manager.pad_active[i] = connected;
        if (notification_timeout[i] > 0)
        {
            notification_timeout[i] -= delta_time;
        }
    }
}

static void render(void)
{
    int count = 0;
    for (int i = 0; i < MAX_INPUTS; i++)
    {
        if (notification_timeout[i] > 0)
        {
            count++;
        }
    }

    for (int i = 0; i < MAX_INPUTS; i++)
    {
        if (notification_timeout[i] > 0)
        {
            int rect_height = 15;
            int y_position = (int)settings.game.target_size.y - (count * rect_height);

            const char *text = TextFormat("Controller %d has %s", i + 1, manager.pad_active[i] ? "connected" : "disconnected");
            Vector2 text_size = MeasureTextEx(*font, text, 7, 0.0f);
            Vector2 text_position = {
                (settings.game.target_size.x - text_size.x) / 2,
                y_position + (rect_height - text_size.y) / 2};

            DrawRectangle(0, y_position, (int)settings.game.target_size.x, rect_height, settings.colors.black_alpha_05);
            DrawTextEx(*font, text, text_position, 7, 0.0f, WHITE);
            count--;
        }
    }
}

static int input_pressed(void)
{
    bool key_pressed = GetKeyPressed() != 0;
    if (key_pressed || GetGamepadButtonPressed() != 0)
    {
        for (int i = 0; i < MAX_INPUTS; i++)
        {
            if (key_pressed && (IsKeyPressed(manager.inputs[i].action_k_DOWN) ||
                                IsKeyPressed(manager.inputs[i].action_k_UP) ||
                                IsKeyPressed(manager.inputs[i].action_k_LEFT) ||
                                IsKeyPressed(manager.inputs[i].action_k_RIGHT) ||
                                IsKeyPressed(manager.inputs[i].action_k_ENTER) ||
                                IsKeyPressed(manager.inputs[i].action_k_ESCAPE)))
            {
                TraceLog(LOG_INFO, "Keyboard input pressed: %d", i);
                return i;
            }

            if (manager.pad_active[i] && (IsGamepadButtonPressed(i, manager.inputs[i].action_DOWN) ||  //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_UP) ||    //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_LEFT) ||  //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_RIGHT) || //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_A) ||     //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_B) ||     //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_START) || //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_L) ||     //
                                          IsGamepadButtonPressed(i, manager.inputs[i].action_R)))
            {
                TraceLog(LOG_INFO, "Gamepad input pressed: %d", i);
                return i;
            }
        }
    }

    return -1;
}

InputManager *create_input_manager(void)
{
    manager.inputs = settings.inputs;

    font = resource_manager.get_pixel7_font();

    for (int i = 0; i < MAX_INPUTS; i++)
    {
        notification_timeout[i] = 0.0f;
    }

    manager.update = update;
    manager.render = render;
    manager.input_pressed = input_pressed;

    return &manager;
}