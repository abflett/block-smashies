#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <stdbool.h>
#include "raylib.h"
#include "input_mapping.h"

// ConfigSettings: Savable settings in the JSON file (e.g., screen, fullscreen, volume)
typedef struct
{
    Vector2 screen_size;
    bool fullscreen;
    float volume;
    float sound_fx;
    bool shake_screen;
} ConfigSettings;

typedef struct FileLocations
{
    const char *settings_file;
    const char *resource_file;
    const char *slides_file;
    const char *high_score_file;
    const char *game_data_file;
    const char *levels_file;
    const char *upgrade_file;
} FileLocations;

typedef struct Colors
{
    const Color clear_color;
    const Color screen_color;
    const Color screen_text_color;

    const Color blue_01;
    const Color blue_02;
    const Color blue_03;
    const Color blue_04;
    const Color blue_05;
    const Color red_02;
    const Color red_04; // more of an orange color
    const Color green_04;

    const Color black_alpha_05;

    const Color alpha_00;
    const Color alpha_01;
    const Color alpha_02;
    const Color alpha_03;
    const Color alpha_04;
    const Color alpha_05;
    const Color alpha_06;
    const Color alpha_07;
    const Color alpha_08;
    const Color alpha_09;
    const Color alpha_10;
} Colors;

// GameSettings: Constants and values like target width/height, logo screen time, etc.
typedef struct
{
    const Vector2 target_size;
    const float max_screen_time;
    const float min_screen_time;
    const Rectangle play_area;
} GameSettings;

// GameplaySettings: Playfield-related settings (paddle, bricks, etc.)
typedef struct
{
    // ship
    const float ship_padding_height;
    const float boost_timer;
    const float pulse_timer;
    const float paddle_mass;
    // nanite
    const float nanite_y_velocity;
    // kill boundary
    const float boundary_padding; // segment extra padding for corners
    // walls boundary
    const float edge_padding; //  bottom offset
} GameplaySettings;

// Main struct that holds all categories of settings
typedef struct
{
    ConfigSettings config;
    FileLocations file_locations;
    GameSettings game;
    GameplaySettings gameplay;
    Colors colors;
    InputMapping inputs[4];
    void (*save)(void);
    void (*set_default_inputs)(void);
    void (*set_defaults)(void);
} Settings;

extern Settings settings;

void initialize_settings();

#endif