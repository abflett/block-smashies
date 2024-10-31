#include "math.h"
#include "text_manager.h"
#include "resource_manager.h"

TextManager text_manager;

static void render(const char *text, Vector2 position, Color color)
{
    DrawTextEx(*text_manager.font, text, position, 7, 0.0f, color);
}

static void render_centered(const char *text, Vector2 position, Color color)
{
    Vector2 size = MeasureTextEx(*text_manager.font, text, 7, 0.0f);
    Vector2 center_position = (Vector2){
        roundf(position.x - size.x / 2),
        roundf(position.y - size.y / 2)};
    DrawTextEx(*text_manager.font, text, center_position, 7, 0.0f, color);
}

static void render_ex(const char *text, Vector2 position, float font_size, Color color)
{
    DrawTextEx(*text_manager.font, text, position, font_size, 0.0f, color);
}

static void render_centered_ex(const char *text, Vector2 position, float font_size, Color color)
{
    Vector2 size = MeasureTextEx(*text_manager.font, text, font_size, 0.0f);
    Vector2 center_position = (Vector2){
        roundf(position.x - size.x / 2),
        roundf(position.y - size.y / 2)};
    DrawTextEx(*text_manager.font, text, center_position, font_size, 0.0f, color);
}

void initialize_text_manager(void)
{
    text_manager.font = resource_manager.get_pixel7_font();
    text_manager.render = render;
    text_manager.render_centered = render_centered;
    text_manager.render_ex = render_ex;
    text_manager.render_centered_ex = render_centered_ex;
}

TextManager *get_text_manager(void)
{
    return &text_manager;
}
