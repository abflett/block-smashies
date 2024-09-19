#include "parson.h"
#include "game_settings.h"

GameSettings game_settings;

void save_settings(const char *filename)
{
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_number(root_object, "screen_width", game_settings.screen_width);
    json_object_set_number(root_object, "screen_height", game_settings.screen_height);
    json_object_set_boolean(root_object, "fullscreen", game_settings.fullscreen);
    json_object_set_number(root_object, "volume", game_settings.volume);

    json_serialize_to_file_pretty(root_value, filename);
    json_value_free(root_value);
}

void default_settings(void)
{
    // save in file
    game_settings.screen_width = 1280;
    game_settings.screen_height = 720;
    game_settings.fullscreen = false;
    game_settings.volume = 1.0f;

    // variables
    game_settings.target_width = 320;
    game_settings.target_height = 180;
    game_settings.logo_screen_time = 4.0f;
    game_settings.play_area = (Rectangle){95, 7, 218, 167};
}

void initialize_game_settings(const char *filename)
{
    game_settings.filename = filename;

    JSON_Value *root_value = json_parse_file(game_settings.filename);
    if (root_value == NULL)
    {
        default_settings();
        save_settings(game_settings.filename);
        return;
    }

    JSON_Object *root_object = json_value_get_object(root_value);
    game_settings.screen_width = (int)json_object_get_number(root_object, "screen_width");
    game_settings.screen_height = (int)json_object_get_number(root_object, "screen_height");
    game_settings.fullscreen = json_object_get_boolean(root_object, "fullscreen");
    game_settings.volume = (float)json_object_get_number(root_object, "volume");

    game_settings.target_width = 320;
    game_settings.target_height = 180;
    game_settings.logo_screen_time = 4.0f;
    game_settings.play_area = (Rectangle){95, 7, 218, 167}; // {104, 16, 200, 156}

    json_value_free(root_value);
}

// void save_settings(const char *filename) {
//     JSON_Value *root_value = json_value_init_object();
//     JSON_Object *root_object = json_value_get_object(root_value);

//     // ConfigSettings - savable settings
//     json_object_set_number(root_object, "screen_width", game_settings.config.screen_width);
//     json_object_set_number(root_object, "screen_height", game_settings.config.screen_height);
//     json_object_set_boolean(root_object, "fullscreen", game_settings.config.fullscreen);
//     json_object_set_number(root_object, "volume", game_settings.config.volume);

//     // Serialize and save to file
//     json_serialize_to_file_pretty(root_value, filename);
//     json_value_free(root_value);
// }

// // Set default settings for the game
// void default_settings(void) {
//     // ConfigSettings
//     game_settings.config.screen_width = 1280;
//     game_settings.config.screen_height = 720;
//     game_settings.config.fullscreen = false;
//     game_settings.config.volume = 1.0f;

//     // GameSettings
//     game_settings.game.target_width = 320;
//     game_settings.game.target_height = 180;
//     game_settings.game.logo_screen_time = 4.0f;
//     game_settings.game.play_area = (Rectangle){95, 7, 218, 167};

//     // GameplaySettings
//     game_settings.gameplay.paddle_height = 16.0f;
//     game_settings.gameplay.boost_timer = 0.15f;
//     game_settings.gameplay.boost_active_timer = 1.0f;
//     game_settings.gameplay.pulse_timer = 0.09f;
//     game_settings.gameplay.pulse_active_timer = 1.0f;
//     game_settings.gameplay.paddle_mass = 1.0f;
//     game_settings.gameplay.health_modifier = 1.0f;
//     game_settings.gameplay.currency_modifier = 1.0f;
//     game_settings.gameplay.nanite_y_velocity = 20.0f;
//     game_settings.gameplay.boundary_padding = 4.0f;
//     game_settings.gameplay.edge_padding = 10.0f;
// }

// // Initialize settings (load from file or set defaults)
// void initialize_game_settings(const char *filename) {
//     game_settings.filename = filename;

//     JSON_Value *root_value = json_parse_file(game_settings.filename);
//     if (root_value == NULL) {
//         // If file doesn't exist or can't be read, use default settings
//         default_settings();
//         save_settings(game_settings.filename); // Save default settings to file
//         return;
//     }

//     // Parse the settings from the JSON file
//     JSON_Object *root_object = json_value_get_object(root_value);

//     // ConfigSettings
//     game_settings.config.screen_width = (int)json_object_get_number(root_object, "screen_width");
//     game_settings.config.screen_height = (int)json_object_get_number(root_object, "screen_height");
//     game_settings.config.fullscreen = json_object_get_boolean(root_object, "fullscreen");
//     game_settings.config.volume = (float)json_object_get_number(root_object, "volume");

//     // Set constants manually for GameSettings and GameplaySettings
//     game_settings.game.target_width = 320;
//     game_settings.game.target_height = 180;
//     game_settings.game.logo_screen_time = 4.0f;
//     game_settings.game.play_area = (Rectangle){95, 7, 218, 167};

//     game_settings.gameplay.paddle_height = 16.0f;
//     game_settings.gameplay.boost_timer = 0.15f;
//     game_settings.gameplay.boost_active_timer = 1.0f;
//     game_settings.gameplay.pulse_timer = 0.09f;
//     game_settings.gameplay.pulse_active_timer = 1.0f;
//     game_settings.gameplay.paddle_mass = 1.0f;
//     game_settings.gameplay.health_modifier = 1.0f;
//     game_settings.gameplay.currency_modifier = 1.0f;
//     game_settings.gameplay.nanite_y_velocity = 20.0f;
//     game_settings.gameplay.boundary_padding = 4.0f;
//     game_settings.gameplay.edge_padding = 10.0f;

//     // Free the root value after parsing
//     json_value_free(root_value);
// }