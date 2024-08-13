#include "raylib.h"
#include "parson.h"
#include "game_settings.h"

GameSettings game_settings;

void load_settings(const char *filename)
{
    JSON_Value *root_value = json_parse_file(filename);
    if (root_value == NULL)
    {
        set_default_settings();
        save_settings(filename);
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
    game_settings.exitWindow = false;

    json_value_free(root_value);
}

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

void set_default_settings(void)
{
    game_settings.screen_width = 1280;
    game_settings.screen_height = 720;
    game_settings.fullscreen = false;
    game_settings.volume = 1.0f;
    game_settings.exitWindow = false;
    game_settings.target_width = 320;
    game_settings.target_height = 180;
    game_settings.logo_screen_time = 4.0f;
}

void init_game_from_settings(const char *settings_file)
{
    load_settings(settings_file);
    InitWindow(game_settings.screen_width, game_settings.screen_height, "Block Smashies");

    // Todo: Allow other available full screen resolutions other then native
    if (game_settings.fullscreen)
    {
        int monitor = GetCurrentMonitor();
        game_settings.screen_width = GetMonitorWidth(monitor);
        game_settings.screen_height = GetMonitorHeight(monitor);

        SetWindowSize(game_settings.screen_width, game_settings.screen_height);
        ToggleFullscreen();
    }
}