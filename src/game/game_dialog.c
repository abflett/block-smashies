#include <string.h>
#include <stdlib.h>
#include "game_dialog.h"
#include "resource_manager.h"
#include "settings.h"

static void update(GameDialog *game_dialog, const float delta_time)
{
    if (!game_dialog->active)
        return;

    if (IsKeyPressed(KEY_ENTER))
    {
        game_dialog->result = true;
        game_dialog->active = false;
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        game_dialog->result = false;
        game_dialog->active = false;
    }
}

static void render(const GameDialog *game_dialog)
{
    if (!game_dialog || !game_dialog->active || !game_dialog->text_manager)
        return; // Error checking

    Vector2 dialog_offset = (Vector2){0.0f, 0.0f};

    DrawRectangle(0, 0, (int)settings.game.target_size.x, (int)settings.game.target_size.y, settings.colors.black_alpha_05);
    DrawTexture(*game_dialog->panel_bg, (int)(dialog_offset.x + game_dialog->position.x), (int)(dialog_offset.y + game_dialog->position.y), WHITE);
}

static void cleanup(GameDialog *game_dialog)
{
    if (game_dialog)
    {                               // Check if game_dialog is not NULL
        free(game_dialog->message); // Free the allocated message
        free(game_dialog);          // Free the GameDialog structure itself
    }
}

static void activate(GameDialog *game_dialog)
{
    game_dialog->active = true;
}

static bool get_result(const GameDialog *game_dialog)
{
    return game_dialog->result;
}

// Todo: add yes and no text
GameDialog *create_game_dialog(const char *message, const Vector2 position)
{
    GameDialog *game_dialog = (GameDialog *)malloc(sizeof(GameDialog));
    if (!game_dialog)
    {
        return NULL; // Check allocation
    }

    game_dialog->active = true;
    game_dialog->message = strdup(message);
    if (!game_dialog->message)
    {
        free(game_dialog); // Clean up if allocation fails
        return NULL;       // Allocation failed
    }

    game_dialog->text_manager = get_text_manager();
    game_dialog->position = position;

    game_dialog->panel_bg = &resource_manager.get_texture("dialog-panel")->texture;
    game_dialog->btn = &resource_manager.get_texture("dialog-btn")->texture;
    game_dialog->btn_lit = &resource_manager.get_texture("dialog-btn-lit")->texture;

    // Assign function pointers
    game_dialog->update = update;
    game_dialog->render = render;
    game_dialog->cleanup = cleanup;
    game_dialog->activate = activate;
    game_dialog->get_result = get_result;

    return game_dialog;
}