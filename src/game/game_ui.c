#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "game_ui.h"
#include "resource_manager.h"
#include "settings.h"
#include "playing_ui_bars.h"

#define NUM_DEBRIS 4

static GameUi game_ui;
static PlayingUiBars *ui_bars;
Debris debris_array[NUM_DEBRIS];
static GameStatus *status;
static Texture2D *screen_bg;
static Texture2D *foreground;
static Texture2D *radar_fade;
static Texture2D *rings;
static Texture2D *needle;
static Texture2D *clock;
static Texture2D *nanite;
static Texture2D *life;
static Texture2D *score_ui;
static Texture2D *debris;
static Texture2D *menu_screen;

static float radar_rotation = 0.0f;
static unsigned char combo1_led_pattern = 0;
static unsigned char random_led_pattern = 0;
static unsigned char color_led_pattern = 0;
static bool toggle_green_led = false;
static float combo1_accumulator = 0.0f;
static float combo2_accumulator = 0.0f;
static float combo3_accumulator = 0.0f;
static bool toggle_menu = false;
static char time_text[10];
static char currency_text[21];
static char score_text[21];

int minutes = 0;
float seconds = 0;

void init_debris(Rectangle play_area)
{
    for (int i = 0; i < NUM_DEBRIS; i++)
    {
        debris_array[i].x = (float)GetRandomValue((int)play_area.x, (int)play_area.x + (int)play_area.width);
        debris_array[i].y = (float)GetRandomValue((int)play_area.y, (int)play_area.y + (int)play_area.height);
        debris_array[i].fall_speed = (float)GetRandomValue(100, 300) / 100.0f; // Speed between 1.0 and 3.0
    }
}

void update_debris(Rectangle play_area)
{
    for (int i = 0; i < NUM_DEBRIS; i++)
    {
        debris_array[i].y += debris_array[i].fall_speed;

        // If debris goes beyond the bottom of the play area, reset to top
        if (debris_array[i].y > play_area.y + play_area.height)
        {
            debris_array[i].x = (float)GetRandomValue((int)play_area.x, (int)play_area.x + (int)play_area.width);
            debris_array[i].y = play_area.y; // Reset to the top of the screen
        }
    }
}

static void render_leds(void)
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
        toggle_menu = !toggle_menu;
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

    if (toggle_menu)
    {
        menu_screen = &resource_manager.get_texture("gameplay-ui-menu-01")->texture;
    }
    else
    {
        menu_screen = &resource_manager.get_texture("gameplay-ui-menu-02")->texture;
    }

    minutes = (int)(status->game_time / 60);
    seconds = status->game_time - minutes * 60;
    snprintf(time_text, sizeof(time_text), "%d:%05.2f", minutes, seconds);
    snprintf(currency_text, sizeof(currency_text), "%0.2f", status->currency);
    snprintf(score_text, sizeof(score_text), "%d", status->score);

    // In the game update loop
    update_debris(settings.game.play_area);
    ui_bars->update(delta_time);
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

    DrawTexture(*menu_screen, 5, 68, WHITE);

    // Operation number
    DrawTextEx(*resource_manager.get_pixel7_font(), "07", (Vector2){35.0f, 70.0f}, 7, 0.0f, settings.colors.blue_03);

    // Mission number
    DrawTextEx(*resource_manager.get_pixel7_font(), "4", (Vector2){48.0f, 70.0f}, 7, 0.0f, settings.colors.blue_03);

    // Game Status
    DrawTexture(*clock, 18, 12, WHITE);
    DrawTextEx(*resource_manager.get_pixel7_font(), time_text, (Vector2){26.0f, 11.0f}, 7, 0, settings.colors.screen_text_color);

    DrawTexture(*nanite, 18, 24, WHITE);
    DrawTextEx(*resource_manager.get_pixel7_font(), currency_text, (Vector2){26.0f, 24.0f}, 7, 0, settings.colors.screen_text_color);

    DrawTexture(*score_ui, 18, 36, WHITE);
    DrawTextEx(*resource_manager.get_pixel7_font(), score_text, (Vector2){26.0f, 36.0f}, 7, 0, settings.colors.screen_text_color);

    for (int i = 0; i < status->lives; i++)
    {
        DrawTexture(resource_manager.get_texture("life-ui")->texture, (i * 9) + 18, 48, WHITE);
    }

    for (int i = 0; i < NUM_DEBRIS; i++)
    {
        DrawTexture(*debris, (int)debris_array[i].x, (int)debris_array[i].y, WHITE);
    }
}

static void render_after_content_ui(void)
{
    DrawTexture(*foreground, 0, 0, WHITE);
    render_leds();

    // DrawTexturePro(bar_a->texture_resource->texture, bar_a->src, bar_a->src, (Vector2){0, 0}, 0.0f, WHITE);

    ui_bars->render();
}

static void cleanup_ui(void)
{
}

GameUi *create_game_ui(GameStatus *game_status)
{
    status = game_status;
    screen_bg = &resource_manager.get_texture("gameplay-screen-bg-01")->texture;
    foreground = &resource_manager.get_texture("gameplay-fg")->texture;

    radar_fade = &resource_manager.get_texture("gameplay-ui-radar-fade")->texture;
    rings = &resource_manager.get_texture("gameplay-ui-rings")->texture;
    needle = &resource_manager.get_texture("gameplay-ui-needle")->texture;

    menu_screen = &resource_manager.get_texture("gameplay-ui-menu-01")->texture;

    clock = &resource_manager.get_texture("clock-ui")->texture;
    nanite = &resource_manager.get_texture("nanite-ui")->texture;
    life = &resource_manager.get_texture("life-ui")->texture;
    score_ui = &resource_manager.get_texture("score-ui")->texture;

    debris = &resource_manager.get_texture("gameplay-ui-debris")->texture;

    init_debris(settings.game.play_area);

    ui_bars = create_playing_ui_bars(game_status);

    // set defaults
    minutes = 0;
    seconds = 0;
    snprintf(time_text, sizeof(time_text), "%d:%05.2f", minutes, seconds);
    snprintf(currency_text, sizeof(currency_text), "%0.2f", game_status->currency);
    snprintf(score_text, sizeof(score_text), "%d", game_status->score);

    game_ui.update = update_ui;
    game_ui.render_before_content = render_before_content_ui;
    game_ui.render_after_content = render_after_content_ui;
    game_ui.cleanup = cleanup_ui;

    return &game_ui;
}
