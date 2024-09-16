#include "raylib.h"
#include "pause_menu_state.h"
#include "game_state_manager.h"
#include "resource_manager.h"
#include "scene_manager.h"
#include "game_settings.h"
#include "entities.h"
#include "resource_manager.h"

Entities *entities;
Texture2D background;

static void state_init(void)
{
    background = resource_manager.get_texture("gameplay-bg")->texture;
    entities = &game_state_manager.context->entities;
}

static void state_update(float delta_time)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (game_state_manager.states.playing->cleanup)
        {
            game_state_manager.states.playing->cleanup();
        }

        scene_manager.next_scene = scene_manager.scenes.logo;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        game_state_manager.change(game_state_manager.states.playing);
    }
}

static void state_render(void)
{
    DrawTexture(background, 0, 0, WHITE);
    entities->render(entities);

    DrawRectangle(0, game_settings.target_height / 2 - 40, game_settings.target_width, 80, (Color){0, 0, 0, 170});
    // Text to be drawn
    const char *text = "'Escape' to exit\n'Enter' to continue playing!";

    // Font size
    int fontSize = 8;

    // Measure the width of the text
    int textWidth = MeasureText(text, fontSize);

    // Calculate the position to center the text horizontally
    int posX = (game_settings.target_width - textWidth) / 2;

    // Calculate the position to center the text vertically (within the rectangle)
    int posY = (game_settings.target_height / 2) - (fontSize / 2) - fontSize;

    // Draw the text at the calculated position
    DrawText(text, posX, posY, fontSize, LIGHTGRAY);
}

static void state_cleanup(void)
{
}

GameState pause_menu_state = {
    .init = state_init,
    .update = state_update,
    .render = state_render,
    .cleanup = state_cleanup,
};