#include "raylib.h"
#include "game_settings.h"

void set_game_resolution(int *screen_width, int *screen_height, bool fullscreen)
{
    InitWindow(*screen_width, *screen_height, "Block Smashies");

    // Set fullscreen mode if requested
    if (fullscreen)
    {
        int monitor = GetCurrentMonitor();
        *screen_width = GetMonitorWidth(monitor);
        *screen_height = GetMonitorHeight(monitor);

        SetWindowSize(*screen_width, *screen_height);
        ToggleFullscreen();
    }
}