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
