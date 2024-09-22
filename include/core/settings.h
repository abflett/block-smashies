#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <stdbool.h>
#include "raylib.h"

// ConfigSettings: Savable settings in the JSON file (e.g., screen, fullscreen, volume)
typedef struct
{
    Vector2 screen_size;
    bool fullscreen;
    float volume;
    float sound_fx;
} ConfigSettings;

// GameSettings: Constants and values like target width/height, logo screen time, etc.
typedef struct
{
    const Vector2 target_size;
    const float logo_screen_time;
    const Rectangle play_area;
    const Color clear_color;
    const Color screen_color;
    const Color screen_text_color;
} GameSettings;

// GameplaySettings: Playfield-related settings (paddle, bricks, etc.)
typedef struct
{
    const float paddle_height;
    const float boost_timer;
    const float boost_active_timer;
    const float pulse_timer;
    const float pulse_active_timer;
    const float paddle_mass;
    const float health_modifier;
    const float currency_modifier;
    const float nanite_y_velocity;
    const float boundary_padding;
    const float edge_padding;
} GameplaySettings;

// Main struct that holds all categories of settings
typedef struct
{
    ConfigSettings config;
    GameSettings game;
    GameplaySettings gameplay;
    const char *filename;
    void (*save)(void);
    void (*set_defaults)(void);
} Settings;

extern Settings settings;

void initialize_settings();

#endif