#include "raylib.h"
#include "logo_scene.h"
#include "settings.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "input_manager.h"
#include "upgrade_manager.h"
#include "game_data.h"

#define MAX_PLAYERS 4

static InputManager *input_manager;
static Scene logo_scene;
static float max_scene_time; // Variable to track elapsed time
static float min_scene_time; // Variable to track elapsed time
static Font *font;

static void scene_init(int arg_count, va_list args)
{
    max_scene_time = settings.game.max_screen_time;
    min_scene_time = settings.game.min_screen_time;

    UpgradeManager *upgrade_manager = create_upgrade_manager(create_game_data());
}

static void scene_update(float delta_time)
{
    if (min_scene_time >= 0)
    {
        min_scene_time -= delta_time;
    }

    if (max_scene_time >= 0)
    {
        max_scene_time -= delta_time;
    }

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        InputMapping *input = input_manager->get_player_input(i);
        int mapping = input_manager->player[i];

        if (input_manager->key_debounce(mapping, input->action_k_ENTER) || //
            input_manager->button_debounce(mapping, input->action_A) ||
            input_manager->button_debounce(mapping, input->action_B) ||
            input_manager->button_debounce(mapping, input->action_START) ||
            max_scene_time <= 0.0f && min_scene_time <= 0.0f)
        {
            scene_manager.change(scene_manager.scenes.title, 0);
            break;
        }
    }
}

static void scene_render(void)
{
    const char *text = "Press any button";
    Vector2 text_size = MeasureTextEx(*font, text, 8, 0.0f);
    Vector2 text_position = {
        (settings.game.target_size.x - text_size.x) / 2,
        (settings.game.target_size.y - text_size.y) / 2};
    DrawTextEx(*font, text, text_position, 7, 0.0f, (Color){171, 148, 122, 255});
}

static void scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene *create_logo_scene()
{
    font = resource_manager.get_pixel7_font();
    input_manager = get_input_manager();
    logo_scene.init = scene_init;
    logo_scene.update = scene_update;
    logo_scene.render = scene_render;
    logo_scene.cleanup = scene_cleanup;

    return &logo_scene;
};