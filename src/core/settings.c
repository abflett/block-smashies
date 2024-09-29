#include "parson.h"
#include "settings.h"

static void save_settings(void);
static void set_default_config_settings(void);

Settings settings = {
    .game = {
        .target_size = {320, 180},
        .play_area = {95, 7, 218, 167},
        .logo_screen_time = 4.0f},

    .file_locations = {
        .game_data_file = "assets/config/game_data.json",    // save files
        .high_score_file = "assets/config/high_scores.json", // high scores
        .resource_file = "assets/config/resources.json",     // asset locations
        .settings_file = "assets/config/settings.json",      // save settings and options
        .levels_file = "assets/config/levels.json"           // levels data
    },

    .colors = {                                   // colors
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
               .alpha_03 = {255, 255, 255, 200},
               .alpha_06 = {255, 255, 255, 180},
               .alpha_05 = {255, 255, 255, 64},
               .alpha_07 = {255, 255, 255, 32}},

    .gameplay = {
        .paddle_height = 16.0f,
        .boost_timer = 0.15f,
        .boost_active_timer = 1.0f,
        .pulse_timer = 0.09f,
        .pulse_active_timer = 1.0f,
        .paddle_mass = 1.0f,
        .health_modifier = 1.0f,    // brick health multiplier based on brick type
        .currency_modifier = 1.0f,  // brick currency multiplier based on brick type
        .nanite_y_velocity = 20.0f, // velocity that nanites fall
        .boundary_padding = 4.0f,   // extra big segment line for kill boundary
        .edge_padding = 10.0f,
    },

    .set_defaults = set_default_config_settings,
    .save = save_settings};

static void save_settings(void)
{
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_number(root_object, "screen_width", settings.config.screen_size.x);
    json_object_set_number(root_object, "screen_height", settings.config.screen_size.y);
    json_object_set_boolean(root_object, "fullscreen", settings.config.fullscreen);
    json_object_set_number(root_object, "volume", settings.config.volume);
    json_object_set_number(root_object, "sound_fx", settings.config.sound_fx);
    json_object_set_boolean(root_object, "shake_screen", settings.config.shake_screen);

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

void initialize_settings()
{
    // ConfigSettings
    JSON_Value *root_value = json_parse_file(settings.file_locations.settings_file);
    if (root_value == NULL)
    {
        settings.set_defaults();
        settings.save();
        return;
    }

    JSON_Object *root_object = json_value_get_object(root_value);
    settings.config.screen_size.x = (float)json_object_get_number(root_object, "screen_width");
    settings.config.screen_size.y = (float)json_object_get_number(root_object, "screen_height");
    settings.config.fullscreen = json_object_get_boolean(root_object, "fullscreen");
    settings.config.volume = (float)json_object_get_number(root_object, "volume");
    settings.config.sound_fx = (float)json_object_get_number(root_object, "sound_fx");
    settings.config.shake_screen = json_object_get_boolean(root_object, "shake_screen");
    json_value_free(root_value);
}