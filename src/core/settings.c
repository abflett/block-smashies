#include "parson.h"
#include "settings.h"

static void save_settings(void);
static void set_default_config_settings(void);

Settings settings = {
    .game = {
        .target_size = {320, 180},
        .play_area = {95, 7, 218, 167},
        .logo_screen_time = 4.0f},
    .gameplay = {
        .paddle_height = 16.0f,
        .boost_timer = 0.15f,
        .boost_active_timer = 1.0f,
        .pulse_timer = 0.09f,
        .pulse_active_timer = 1.0f,
        .paddle_mass = 1.0f,
        .health_modifier = 1.0f,
        .currency_modifier = 1.0f,
        .nanite_y_velocity = 20.0f,
        .boundary_padding = 4.0f,
        .edge_padding = 10.0f,
    },
    .filename = "settings.json",
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

    json_serialize_to_file_pretty(root_value, settings.filename);
    json_value_free(root_value);
}

static void set_default_config_settings(void)
{
    settings.config.screen_size = (Vector2){1280, 720};
    settings.config.fullscreen = false;
    settings.config.volume = 1.0f;
    settings.config.sound_fx = 1.0f;
}

void initialize_settings()
{
    // ConfigSettings
    JSON_Value *root_value = json_parse_file(settings.filename);
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

    json_value_free(root_value);
}