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
    if (!game_dialog || !game_dialog->active || !game_dialog->font)
        return; // Error checking

    const Vector2 pos = game_dialog->position;
    const Vector2 size = {200.f, 60.f}; // Size of the dialog box

    // Draw the dialog box
    DrawRectangle((int)pos.x, (int)pos.y, (int)size.x, (int)size.y, (Color){0, 0, 0, 200});

    // Measure text size and calculate initial position
    const float line_height = 7.0f;           // Height of each line of text
    Vector2 msg_pos = {pos.x + 5, pos.y + 5}; // Initial position with padding

    // Split the message into lines at new line characters
    // Todo: move to stack if possible
    char *message_copy = strdup(game_dialog->message); // Create a mutable copy of the message
    const char *line = strtok(message_copy, "\n");           // Split by new line

    // Draw each line of the message
    while (line)
    {
        // Measure the width of the current line
        const float line_width = MeasureTextEx(*game_dialog->font, line, line_height, 0.0f).x;

        // Center the line within the dialog box
        msg_pos.x = pos.x + (size.x - line_width) * 0.5f; // Center horizontally

        // Draw the current line
        DrawTextEx(*game_dialog->font, line, msg_pos, line_height, 0.0f, WHITE);
        msg_pos.y += line_height;  // Move down for the next line
        line = strtok(NULL, "\n"); // Get the next line
    }

    // Clean up
    free(message_copy); // Free the mutable copy

    // Define button dimensions and spacing
    const Vector2 btn_size = {50.f, 10.f};

    // Calculate button positions
    const char *buttons[] = {"No", "Yes"};
    const Color bkg = WHITE;
    const Color txt = BLACK;

    // Adjust button positions based on the number of lines drawn
    for (int i = 0; i < 2; ++i)
    {
        const float btn_space = 20.f;
        const float x_pos = pos.x + (size.x - ((btn_size.x * 2) + btn_space)) * 0.5f + (btn_size.x + btn_space) * (float)i;
        DrawRectangle((int)x_pos, (int)(pos.y + 45 + (line_height * (msg_pos.y / line_height))), (int)btn_size.x, (int)btn_size.y, bkg);
        const Vector2 text_pos = {x_pos + (btn_size.x - MeasureTextEx(*game_dialog->font, buttons[i], 7, 0).x) * 0.5f, pos.y + 46 + (line_height * (msg_pos.y / line_height))};
        DrawTextEx(*game_dialog->font, buttons[i], text_pos, 7, 0.0f, txt);
    }
}

static void cleanup(struct GameDialog *game_dialog)
{
    if (game_dialog)
    {                               // Check if game_dialog is not NULL
        free(game_dialog->message); // Free the allocated message
        free(game_dialog);          // Free the GameDialog structure itself
    }
}

static void activate(struct GameDialog *game_dialog)
{
    game_dialog->active = true;
}

static bool get_result(struct GameDialog *game_dialog)
{
    return false;
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
    game_dialog->font = resource_manager.get_pixel7_font();
    game_dialog->position = position;

    // Assign function pointers
    game_dialog->update = update;
    game_dialog->render = render;
    game_dialog->cleanup = cleanup;
    game_dialog->activate = activate;
    game_dialog->get_result = get_result;

    return game_dialog;
}