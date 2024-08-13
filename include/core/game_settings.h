#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

typedef struct
{
    int screen_width;
    int screen_height;
    bool fullscreen;
    float volume;
    bool exitWindow;
    int target_width;
    int target_height;
    float logo_screen_time;
} GameSettings;

extern GameSettings game_settings;

void load_settings(const char *filename);
void save_settings(const char *filename);
void set_default_settings(void);
void init_game_from_settings(const char *settings_file);

#endif