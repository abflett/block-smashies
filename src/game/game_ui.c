#include <math.h>
#include <stdbool.h>
#include "game_ui.h"
#include "resource_manager.h"
#include "settings.h"

static GameUi game_ui;
static Texture2D *screen_bg;
static Texture2D *foreground;
static Texture2D *radar_fade;
static Texture2D *rings;
static Texture2D *needle;

static float radar_rotation = 0.0f;
static unsigned char combo1_led_pattern = 0;
static unsigned char random_led_pattern = 0;
static unsigned char color_led_pattern = 0;
static bool toggle_green_led = false;
static float combo1_accumulator = 0.0f;
static float combo2_accumulator = 0.0f;
static float combo3_accumulator = 0.0f;

static void update_ui(float delta_time)
{
    radar_rotation += delta_time * 50; // Adjust the speed of rotation if necessary
    radar_rotation = fmodf(radar_rotation, 360.0f);

    // Update the LED pattern every second
    combo1_accumulator += delta_time;
    if (combo1_accumulator >= 1.0f)
    {
        combo1_accumulator = 0.0f;
        combo1_led_pattern = (combo1_led_pattern + 1) % 256; // Cycle through 256 patterns (0-255)
        toggle_green_led = !toggle_green_led;
    }

    combo2_accumulator += delta_time;
    if (combo2_accumulator >= 0.4f)
    {
        combo2_accumulator = 0.0f;
        random_led_pattern = GetRandomValue(0, 255);
    }

    combo3_accumulator += delta_time;
    if (combo3_accumulator >= 0.1f)
    {
        combo3_accumulator = 0.0f;
        color_led_pattern = GetRandomValue(0, 255);
    }
}

static render_leds(void)
{
    // Draw LED combo based on combo1_led_pattern
    for (int i = 0; i < 8; i++)
    {
        if ((combo1_led_pattern >> i) & 1)
        {
            DrawPixel(110 + (i * 2), 3, settings.colors.red_02);
        }
    }

    // Draw LED combo based on random_led_pattern
    for (int i = 0; i < 8; i++)
    {
        if ((random_led_pattern >> i) & 1)
        {
            DrawPixel(264 + (i * 6), 1, settings.colors.red_02);
        }
    }

    // large green led
    if (toggle_green_led)
    {
        DrawRectangle(140, 2, 2, 2, settings.colors.green_04);
    }

    // Draw amber LEDs based on the first 3 bits
    for (int i = 0; i < 3; i++)
    {
        if ((color_led_pattern >> i) & 1)
        {
            DrawPixel(262 + (i * 6), 3, settings.colors.red_04);
        }
    }

    // Draw blue LEDs based on the next 2 bits
    for (int i = 0; i < 2; i++)
    {
        if ((color_led_pattern >> (i + 3)) & 1)
        {
            DrawPixel(280 + (i * 6), 3, settings.colors.blue_04);
        }
    }

    // Draw green LEDs based on the last 3 bits
    for (int i = 0; i < 3; i++)
    {
        if ((color_led_pattern >> (i + 5)) & 1)
        {
            DrawPixel(292 + (i * 6), 3, settings.colors.green_04);
        }
    }
}

static void render_before_content_ui(void)
{
    ClearBackground(settings.colors.screen_color);
    DrawTexture(*screen_bg, 95, 7, WHITE); // screen_background

    DrawCircle(38, 130, 3, settings.colors.blue_03); // moon
    DrawCircle(49, 140, 4, settings.colors.blue_03); // planet

    // Radar
    DrawTexturePro(
        *radar_fade,                                                                                                                                  // The texture
        (Rectangle){0, 0, (float)radar_fade->width, (float)radar_fade->height},                                                                       // Source rectangle (entire texture)
        (Rectangle){((float)radar_fade->width / 2) + 31, ((float)radar_fade->height / 2) + 123, (float)radar_fade->width, (float)radar_fade->height}, // Destination rectangle (position on screen)
        (Vector2){(float)radar_fade->width / 2, (float)radar_fade->height / 2},                                                                       // Origin point for rotation (center of the texture)
        radar_rotation,                                                                                                                               // Rotation angle
        WHITE                                                                                                                                         // Tint (full white for no tint)
    );
    DrawTexture(*rings, 34, 126, WHITE);
    DrawTexturePro(
        *needle,                                                                                                                      // The texture
        (Rectangle){0, 0, (float)needle->width, (float)needle->height},                                                               // Source rectangle (entire texture)
        (Rectangle){((float)needle->width / 2) + 31, ((float)needle->height / 2) + 123, (float)needle->width, (float)needle->height}, // Destination rectangle (position on screen)
        (Vector2){(float)needle->width / 2, (float)needle->height / 2},                                                               // Origin point for rotation (center of the texture)
        radar_rotation,                                                                                                               // Rotation angle
        WHITE                                                                                                                         // Tint (full white for no tint)
    );
}

static void render_after_content_ui(void)
{
    DrawTexture(*foreground, 0, 0, WHITE);
    render_leds();
}

static void cleanup_ui(void)
{
}

GameUi *create_game_ui(void)
{
    screen_bg = &resource_manager.get_texture("gameplay-screen-bg-01")->texture;
    foreground = &resource_manager.get_texture("gameplay-fg")->texture;

    radar_fade = &resource_manager.get_texture("gameplay-ui-radar-fade")->texture;
    rings = &resource_manager.get_texture("gameplay-ui-rings")->texture;
    needle = &resource_manager.get_texture("gameplay-ui-needle")->texture;

    game_ui.update = update_ui;
    game_ui.render_before_content = render_before_content_ui;
    game_ui.render_after_content = render_after_content_ui;
    game_ui.cleanup = cleanup_ui;

    return &game_ui;
}
