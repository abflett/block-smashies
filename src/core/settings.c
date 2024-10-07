#include "parson.h"
#include "settings.h"

static void save_settings(void);
static void set_default_config_settings(void);
static void set_default_input_settings(void);

Settings settings = {
    .game = {
        .target_size = {320, 180},
        .play_area = {95, 7, 218, 167},
        .logo_screen_time = 4.0f //
    },

    .file_locations = {
        .game_data_file = "assets/config/game_data.json",    // save files
        .high_score_file = "assets/config/high_scores.json", // high scores
        .resource_file = "assets/config/resources.json",     // asset locations
        .settings_file = "assets/config/settings.json",      // save settings and options
        .levels_file = "assets/config/levels.json"           // levels data
    },

    .colors = {
        .clear_color = {46, 34, 47, 255},  // off black
        .screen_color = {50, 51, 83, 255}, // color of the displays or blue_01
        .screen_text_color = {77, 155, 230, 255},
        .blue_01 = {50, 51, 83, 255},
        .blue_02 = {72, 84, 119, 255},
        .blue_03 = {77, 101, 180, 255},
        .blue_04 = {77, 155, 230, 255},
        .blue_05 = {143, 211, 230, 255},
        .red_02 = {232, 59, 59, 255},
        .red_04 = {247, 150, 23, 255},
        .green_04 = {145, 219, 105, 255},
        .alpha_00 = {255, 255, 255, 0}, // Full
        .alpha_01 = {255, 255, 255, 25},
        .alpha_02 = {255, 255, 255, 51},
        .alpha_03 = {255, 255, 255, 76},
        .alpha_04 = {255, 255, 255, 102},
        .alpha_05 = {255, 255, 255, 127},
        .alpha_06 = {255, 255, 255, 153},
        .alpha_07 = {255, 255, 255, 178},
        .alpha_08 = {255, 255, 255, 204},
        .alpha_09 = {255, 255, 255, 229},
        .alpha_10 = {255, 255, 255, 255} // White
    },

    .gameplay = {
        // paddle
        .paddle_height = 16.0f, //
        .boost_timer = 0.15f,   // double press timer for boosting
        .pulse_timer = 0.09f,   // pulse animation up timer
        .paddle_mass = 1.0f,
        // nanite
        .nanite_y_velocity = 20.0f, // velocity that nanites fall
        // kill boundary
        .boundary_padding = 4.0f, // extra big segment line for kill boundary
        // walls boundary
        .edge_padding = 10.0f //
    },

    .set_default_inputs = set_default_input_settings,
    .set_defaults = set_default_config_settings,
    .save = save_settings};

static void save_settings(void)
{
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    // Save other settings like screen size, fullscreen, etc.
    json_object_set_number(root_object, "screen_width", settings.config.screen_size.x);
    json_object_set_number(root_object, "screen_height", settings.config.screen_size.y);
    json_object_set_boolean(root_object, "fullscreen", settings.config.fullscreen);
    json_object_set_number(root_object, "volume", settings.config.volume);
    json_object_set_number(root_object, "sound_fx", settings.config.sound_fx);
    json_object_set_boolean(root_object, "shake_screen", settings.config.shake_screen);

    // Create JSON array for input mappings
    JSON_Value *input_mappings_value = json_value_init_array();
    JSON_Array *input_mappings_array = json_value_get_array(input_mappings_value);

    // Iterate through each player (0-3)
    for (int i = 0; i < 4; i++)
    {
        JSON_Value *input_mapping_value = json_value_init_object();
        JSON_Object *input_mapping_object = json_value_get_object(input_mapping_value);

        // Save each button and key mapping
        json_object_set_number(input_mapping_object, "action_UP", settings.inputs[i].action_UP);
        json_object_set_number(input_mapping_object, "action_RIGHT", settings.inputs[i].action_RIGHT);
        json_object_set_number(input_mapping_object, "action_DOWN", settings.inputs[i].action_DOWN);
        json_object_set_number(input_mapping_object, "action_LEFT", settings.inputs[i].action_LEFT);
        json_object_set_number(input_mapping_object, "action_A", settings.inputs[i].action_A);
        json_object_set_number(input_mapping_object, "action_B", settings.inputs[i].action_B);
        json_object_set_number(input_mapping_object, "action_START", settings.inputs[i].action_START);

        // Save keyboard mappings
        json_object_set_number(input_mapping_object, "action_k_UP", settings.inputs[i].action_k_UP);
        json_object_set_number(input_mapping_object, "action_k_RIGHT", settings.inputs[i].action_k_RIGHT);
        json_object_set_number(input_mapping_object, "action_k_DOWN", settings.inputs[i].action_k_DOWN);
        json_object_set_number(input_mapping_object, "action_k_LEFT", settings.inputs[i].action_k_LEFT);
        json_object_set_number(input_mapping_object, "action_k_ENTER", settings.inputs[i].action_k_ENTER);
        json_object_set_number(input_mapping_object, "action_k_ESCAPE", settings.inputs[i].action_k_ESCAPE);

        // Save axis mappings
        json_object_set_number(input_mapping_object, "action_a_x", settings.inputs[i].action_a_x);
        json_object_set_number(input_mapping_object, "action_a_y", settings.inputs[i].action_a_y);

        // Add input mapping object to the array
        json_array_append_value(input_mappings_array, input_mapping_value);
    }

    // Add the input mappings array to the root object
    json_object_set_value(root_object, "input_mappings", input_mappings_value);

    json_serialize_to_file_pretty(root_value, settings.file_locations.settings_file);
    json_value_free(root_value);
}

static void set_default_config_settings(void)
{
    settings.config.screen_size = (Vector2){1280, 720};
    settings.config.fullscreen = false;
    settings.config.volume = 1.0f;
    settings.config.sound_fx = 1.0f;
    settings.config.shake_screen = true;
}

static void set_default_input_settings(void)
{
    for (int i = 0; i < 4; i++)
    {
        settings.inputs[i] = (InputMapping){
            .action_UP = GAMEPAD_BUTTON_LEFT_FACE_UP,
            .action_RIGHT = GAMEPAD_BUTTON_LEFT_FACE_RIGHT,
            .action_DOWN = GAMEPAD_BUTTON_LEFT_FACE_DOWN,
            .action_LEFT = GAMEPAD_BUTTON_LEFT_FACE_LEFT,
            .action_A = GAMEPAD_BUTTON_RIGHT_FACE_DOWN,
            .action_B = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,
            .action_START = GAMEPAD_BUTTON_MIDDLE_RIGHT,
            .action_a_x = GAMEPAD_AXIS_LEFT_X,
            .action_a_y = GAMEPAD_AXIS_LEFT_Y};

        // Set different keyboard mappings based on the input index
        switch (i)
        {
        case 0:
            settings.inputs[i].action_k_UP = KEY_UP;
            settings.inputs[i].action_k_DOWN = KEY_DOWN;
            settings.inputs[i].action_k_LEFT = KEY_LEFT;
            settings.inputs[i].action_k_RIGHT = KEY_RIGHT;
            settings.inputs[i].action_k_ENTER = KEY_ENTER;
            settings.inputs[i].action_k_ESCAPE = KEY_ESCAPE;
            break;
        case 1:
            settings.inputs[i].action_k_UP = KEY_W;
            settings.inputs[i].action_k_DOWN = KEY_S;
            settings.inputs[i].action_k_LEFT = KEY_A;
            settings.inputs[i].action_k_RIGHT = KEY_D;
            settings.inputs[i].action_k_ENTER = KEY_E;
            settings.inputs[i].action_k_ESCAPE = KEY_Q;
            break;
        case 2:
            settings.inputs[i].action_k_UP = KEY_T;
            settings.inputs[i].action_k_DOWN = KEY_G;
            settings.inputs[i].action_k_LEFT = KEY_F;
            settings.inputs[i].action_k_RIGHT = KEY_H;
            settings.inputs[i].action_k_ENTER = KEY_Y;
            settings.inputs[i].action_k_ESCAPE = KEY_R;
            break;
        case 3:
            settings.inputs[i].action_k_UP = KEY_I;
            settings.inputs[i].action_k_DOWN = KEY_K;
            settings.inputs[i].action_k_LEFT = KEY_J;
            settings.inputs[i].action_k_RIGHT = KEY_L;
            settings.inputs[i].action_k_ENTER = KEY_O;
            settings.inputs[i].action_k_ESCAPE = KEY_U;
            break;
        }
    }
}

void initialize_settings()
{
    // ConfigSettings
    JSON_Value *root_value = json_parse_file(settings.file_locations.settings_file);
    if (root_value == NULL)
    {
        settings.set_defaults();
        settings.set_default_inputs();
        settings.save();
        return;
    }

    JSON_Object *root_object = json_value_get_object(root_value);

    // Load configuration settings
    settings.config.screen_size.x = (float)json_object_get_number(root_object, "screen_width");
    settings.config.screen_size.y = (float)json_object_get_number(root_object, "screen_height");
    settings.config.fullscreen = json_object_get_boolean(root_object, "fullscreen");
    settings.config.volume = (float)json_object_get_number(root_object, "volume");
    settings.config.sound_fx = (float)json_object_get_number(root_object, "sound_fx");
    settings.config.shake_screen = json_object_get_boolean(root_object, "shake_screen");

    // Load input mappings
    JSON_Array *input_mappings_array = json_object_get_array(root_object, "input_mappings");
    if (input_mappings_array != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            JSON_Object *input_mapping_object = json_array_get_object(input_mappings_array, i);

            // Load gamepad buttons
            settings.inputs[i].action_UP = (GamepadButton)json_object_get_number(input_mapping_object, "action_UP");
            settings.inputs[i].action_RIGHT = (GamepadButton)json_object_get_number(input_mapping_object, "action_RIGHT");
            settings.inputs[i].action_DOWN = (GamepadButton)json_object_get_number(input_mapping_object, "action_DOWN");
            settings.inputs[i].action_LEFT = (GamepadButton)json_object_get_number(input_mapping_object, "action_LEFT");
            settings.inputs[i].action_A = (GamepadButton)json_object_get_number(input_mapping_object, "action_A");
            settings.inputs[i].action_B = (GamepadButton)json_object_get_number(input_mapping_object, "action_B");
            settings.inputs[i].action_START = (GamepadButton)json_object_get_number(input_mapping_object, "action_START");
            // Load gamepad axes
            settings.inputs[i].action_a_x = (GamepadAxis)json_object_get_number(input_mapping_object, "action_a_x");
            settings.inputs[i].action_a_y = (GamepadAxis)json_object_get_number(input_mapping_object, "action_a_y");

            // Load keyboard keys
            settings.inputs[i].action_k_UP = (KeyboardKey)json_object_get_number(input_mapping_object, "action_k_UP");
            settings.inputs[i].action_k_RIGHT = (KeyboardKey)json_object_get_number(input_mapping_object, "action_k_RIGHT");
            settings.inputs[i].action_k_DOWN = (KeyboardKey)json_object_get_number(input_mapping_object, "action_k_DOWN");
            settings.inputs[i].action_k_LEFT = (KeyboardKey)json_object_get_number(input_mapping_object, "action_k_LEFT");
            settings.inputs[i].action_k_ENTER = (KeyboardKey)json_object_get_number(input_mapping_object, "action_k_ENTER");
            settings.inputs[i].action_k_ESCAPE = (KeyboardKey)json_object_get_number(input_mapping_object, "action_k_ESCAPE");
        }
    }

    json_value_free(root_value);
}