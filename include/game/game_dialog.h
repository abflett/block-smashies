#ifndef GAME_DIALOG_H
#define GAME_DIALOG_H

#include <stdbool.h>
#include "raylib.h"
#include "input_manager.h"

typedef struct GameDialog
{
    char *message;
    bool active;
    bool result;

    Font *font;

    InputManager *input_manager;
    Vector2 position;

    void (*update)(struct GameDialog *game_dialog, const float delta_time);
    void (*render)(const struct GameDialog *game_dialog);
    void (*cleanup)(struct GameDialog *game_dialog);
    void (*activate)(struct GameDialog *game_dialog);
    bool (*get_result)(const struct GameDialog *game_dialog);
} GameDialog;

GameDialog *create_game_dialog(const char *message, Vector2 position);

#endif
