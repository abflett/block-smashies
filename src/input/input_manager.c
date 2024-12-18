#include "raylib.h"
#include "input_manager.h"
#include "settings.h"
#include "resource_manager.h"
#include "math.h"

#define NOTIFICATION_TIMEOUT 2.0f

static InputManager manager;
static float notification_timeout[MAX_INPUTS];
static Font *font;
static float axis_debounce_time[MAX_INPUTS];
static float button_debounce_time[MAX_INPUTS];
static float key_debounce_time[MAX_INPUTS];
static float any_button_debounce_time;

static bool key_down_repeat(const int input_index, const KeyboardKey key)
{
    const bool key_pressed = IsKeyDown(key);
    if (key_pressed && key_debounce_time[input_index] <= 0)
    {
        key_debounce_time[input_index] = 0.2f;
        return true;
    }
    return false;
}

static bool key_debounce(const int input_index, const KeyboardKey key)
{
    const bool key_pressed = IsKeyPressed(key);
    if (key_pressed && key_debounce_time[input_index] <= 0)
    {
        key_debounce_time[input_index] = 0.2f;
        return true;
    }
    return false;
}

static bool axis_debounce(const int input_index, const GamepadAxis axis, const float threshold)
{
    const float movement = GetGamepadAxisMovement(input_index, axis);
    const bool exceeds_threshold = (threshold > 0) ? (movement > threshold) : (movement < threshold);

    if (exceeds_threshold && axis_debounce_time[input_index] <= 0)
    {
        axis_debounce_time[input_index] = 0.2f;
        return true;
    }
    return false;
}

static bool button_debounce(const int input_index, const GamepadButton button)
{
    bool button_pressed = IsGamepadButtonPressed(input_index, button);
    if (button_pressed && button_debounce_time[input_index] <= 0)
    {
        button_debounce_time[input_index] = 0.2f;
        return true;
    }
    return false;
}

static bool any_button_debounce(void)
{
    if (GetGamepadButtonPressed() != 0 && any_button_debounce_time <= 0)
    {
        any_button_debounce_time = 0.2f;
        return true;
    }
    return false;
}

static void update(const float delta_time)
{
    if (any_button_debounce_time > 0)
    {
        any_button_debounce_time -= delta_time;
    }

    for (int i = 0; i < MAX_INPUTS; i++)
    {
        if (axis_debounce_time[i] > 0)
        {
            axis_debounce_time[i] -= delta_time;
        }

        if (key_debounce_time[i] > 0)
        {
            key_debounce_time[i] -= delta_time;
        }

        if (button_debounce_time[i] > 0)
        {
            button_debounce_time[i] -= delta_time;
        }

        const bool connected = IsGamepadAvailable(i);
        if (connected != manager.pad_active[i])
        {
            notification_timeout[i] = NOTIFICATION_TIMEOUT;
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
            const float rect_height = 15.0f;
            const float y_position = settings.game.target_size.y - (float)count * rect_height;

            const char *text = TextFormat("Controller %d has %s", i + 1, manager.pad_active[i] ? "connected" : "disconnected");
            const Vector2 text_size = MeasureTextEx(*font, text, 7, 0.0f);
            const Vector2 text_position = {
                (settings.game.target_size.x - text_size.x) / 2,
                y_position + (rect_height - text_size.y) / 2};

            DrawRectangle(0, (int)y_position, (int)settings.game.target_size.x, (int)rect_height, settings.colors.black_alpha_05);
            DrawTextEx(*font, text, text_position, 7, 0.0f, WHITE);
            count--;
        }
    }
}

static int input_pressed(void)
{
    const bool key_pressed = GetKeyPressed() != 0;
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
                return i;
            }
        }
    }

    return -1;
}

static void map_player_input(const int player, const int input_index)
{
    if (player < 0 || player >= MAX_PLAYERS || input_index < 0 || input_index >= MAX_INPUTS)
    {
        return;
    }

    manager.player[player] = input_index;
    manager.player_mapped[player] = true;
    manager.input_mapped[input_index] = true;
}

static bool check_for_new_players(const int player_count)
{
    const int new_input_pressed = manager.input_pressed();
    if (new_input_pressed != -1)
    {
        if (!manager.input_mapped[new_input_pressed])
        {
            manager.map_player_input(player_count, new_input_pressed);
            TraceLog(LOG_INFO, "Player %d added", player_count);
            return true;
        }
    }

    return false;
}

static void unmap_player_input(int player)
{
    if (player < 0 || player >= MAX_PLAYERS)
    {
        return;
    }

    manager.input_mapped[manager.player[player]] = false;

    for (int i = player; i < MAX_PLAYERS - 1; i++)
    {
        manager.player[i] = manager.player[i + 1];
        manager.player_mapped[i] = manager.player_mapped[i + 1];
    }

    manager.player[MAX_PLAYERS - 1] = -1;
    manager.player_mapped[MAX_PLAYERS - 1] = false;
}

static InputMapping *get_player_input(int player)
{
    if (player < 0 || player >= MAX_PLAYERS)
    {
        return NULL;
    }
    return &manager.inputs[manager.player[player]];
}

static void reset_player_inputs(void)
{
    for (int i = 0; i < MAX_INPUTS; i++)
    {
        notification_timeout[i] = 0.0f;
        manager.pad_active[i] = IsGamepadAvailable(i);
        manager.input_mapped[i] = i == 0;
        manager.player_mapped[i] = i == 0;
        manager.player[i] = i == 0 ? 0 : -1;
    }
}

InputManager *get_input_manager(void)
{
    return &manager;
}

InputManager *create_input_manager(void)
{
    font = resource_manager.get_pixel7_font();

    manager.inputs = settings.inputs;

    reset_player_inputs();

    manager.reset_player_inputs = reset_player_inputs;
    manager.key_debounce = key_debounce;
    manager.key_down_repeat = key_down_repeat;
    manager.axis_debounce = axis_debounce;
    manager.button_debounce = button_debounce;
    manager.any_button_debounce = any_button_debounce;
    manager.get_player_input = get_player_input;
    manager.map_player_input = map_player_input;
    manager.unmap_player_input = unmap_player_input;
    manager.update = update;
    manager.render = render;
    manager.input_pressed = input_pressed;
    manager.check_for_new_players = check_for_new_players;

    return &manager;
}