#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <stdbool.h>
#include "raylib.h"

typedef struct
{
    const char *filename;
    int screen_width;
    int screen_height;
    bool fullscreen;
    float volume;
    int target_width;
    int target_height;
    float logo_screen_time;
    Rectangle play_area;

    void (*save)(void);
    void (*set_default)(void);
} GameSettings;

extern GameSettings game_settings;

void initialize_game_settings(const char *filename);

#endif

// // ConfigSettings: Savable settings in the JSON file (e.g., screen, fullscreen, volume)
// typedef struct {
//     int screen_width;
//     int screen_height;
//     bool fullscreen;
//     float volume;
// } ConfigSettings;

// // GameSettings: Constants and values like target width/height, logo screen time, etc.
// typedef struct {
//     int target_width;
//     int target_height;
//     float logo_screen_time;
//     Rectangle play_area;
// } GameSettings;

// // GameplaySettings: Playfield-related settings (paddle, bricks, etc.)
// typedef struct {
//     float paddle_height;
//     float boost_timer;
//     float boost_active_timer;
//     float pulse_timer;
//     float pulse_active_timer;
//     float paddle_mass;
//     float health_modifier;
//     float currency_modifier;
//     float nanite_y_velocity;
//     float boundary_padding;
//     float edge_padding;
// } GameplaySettings;

// // Main struct that holds all categories of settings
// typedef struct {
//     ConfigSettings config;
//     GameSettings game;
//     GameplaySettings gameplay;
//     const char* filename;
// } Settings;

// // Declare a global instance
// extern Settings game_settings;

// // Functions for saving, loading, and initializing settings
// void save_settings(const char *filename);
// void default_settings(void);
// void initialize_game_settings(const char *filename);