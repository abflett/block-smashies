#include "raylib.h"
#include "title_scene.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "settings.h"
#include "input_manager.h"
#include "slide_manager.h"
#include "kvec.h"
#include "b_utils.h"

static Scene title_scene;
static SlideManager *slide_manager;
static InputManager *input_manager;
static float min_scene_time;
static Font *font;

static void scene_init(int arg_count, va_list args)
{
    min_scene_time = settings.game.min_screen_time;

    slide_manager = create_slide_manager("intro");
    if (slide_manager == NULL)
    {
        log_error("Failed to create slide manager for 'intro'");
    }

    TraceLog(LOG_INFO, "Loaded slide manager");
}

static void scene_update(float delta_time)
{
    if (!slide_manager->slides_end)
    {
        slide_manager->update(slide_manager, delta_time);
    }

    if (min_scene_time >= 0.0f)
    {
        min_scene_time -= delta_time;
    }

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        InputMapping *input = input_manager->get_player_input(i);
        int mapping = input_manager->player[i];

        if (input_manager->key_debounce(mapping, input->action_k_ENTER) || //
            input_manager->button_debounce(mapping, input->action_A) ||
            input_manager->button_debounce(mapping, input->action_B) ||
            input_manager->button_debounce(mapping, input->action_START) &&
                min_scene_time <= 0.0f)
        {
            scene_manager.change(scene_manager.scenes.main_menu, 0);
            break;
        }
    }
}

static void scene_render(void)
{

    if (!slide_manager->slides_end)
    {
        slide_manager->render(slide_manager);
    }
    else
    {
        const char *text = "Title: Press any button";
        Vector2 text_size = MeasureTextEx(*font, text, 8, 0.0f);
        Vector2 text_position = {
            (settings.game.target_size.x - text_size.x) / 2,
            (settings.game.target_size.y - text_size.y) / 2};
        DrawTextEx(*font, text, text_position, 7, 0.0f, WHITE);
    }
}

static void scene_cleanup(void)
{
    slide_manager->cleanup(slide_manager);
}

Scene *create_title_scene()
{
    font = resource_manager.get_pixel7_font();
    input_manager = get_input_manager();
    title_scene.init = scene_init;
    title_scene.update = scene_update;
    title_scene.render = scene_render;
    title_scene.cleanup = scene_cleanup;

    return &title_scene;
};