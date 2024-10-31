#ifndef TEXT_MANAGER_H_
#define TEXT_MANAGER_H_

#include "raylib.h"

typedef struct TextManager
{
    Font *font;
    void (*render)(const char *text, Vector2 position, Color color);
    void (*render_centered)(const char *text, Vector2 position, Color color);
    void (*render_ex)(const char *text, Vector2 position, float font_size, Color color);
    void (*render_centered_ex)(const char *text, Vector2 position, float font_size, Color color);
} TextManager;

void initialize_text_manager(void);
TextManager *get_text_manager(void);

#endif
