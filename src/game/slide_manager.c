#include "parson.h"
#include "slide_manager.h"
#include "settings.h"
#include "resource_manager.h"
#include "settings.h"

static void render(SlideManager *manager)
{
    // Draw the texture of the current slide
    DrawTexture(*manager->current_slide->texture, 96, 30, manager->texture_fade_color);

    // Loop through the text lines of the current slide
    for (int i = 0; i < kv_size(manager->current_slide->text_lines); i++)
    {
        SlideText text = kv_A(manager->current_slide->text_lines, i);
        Vector2 text_size = MeasureTextEx(*manager->font, text.text, 7, 0.0f);
        Vector2 text_position = {settings.game.target_size.x / 2 - text_size.x / 2, (float)100 + (i * 10)};
        if (i == manager->current_text_index)
        {
            DrawTextEx(*manager->font, text.text, text_position, 7, 0.0f, manager->text_fade_color);
        }
        else if (i < manager->current_text_index)
        {
            DrawTextEx(*manager->font, text.text, text_position, 7, 0.0f, WHITE);
        }
    }
}

static void update(SlideManager *manager, float delta_time)
{
    if (manager->texture_fade_alpha < 255)
    {
        manager->texture_fade_alpha += delta_time * 200;
        if (manager->texture_fade_alpha >= 255)
        {
            manager->texture_fade_alpha = 255.0f;
        }
    }
    manager->texture_fade_color = (Color){255, 255, 255, (int)manager->texture_fade_alpha};

    if (manager->text_fade_alpha < 255)
    {
        manager->text_fade_alpha += delta_time * 200;
        if (manager->text_fade_alpha >= 255)
        {
            manager->text_fade_alpha = 255.0f;
        }
    }
    manager->text_fade_color = (Color){255, 255, 255, (int)manager->text_fade_alpha};

    // Decrease the remaining time of the current slide
    manager->current_duration += delta_time;
    manager->current_text_duration += delta_time;

    if (manager->current_text_duration >= manager->text_duration)
    {
        if (manager->current_text_index + 1 < kv_size(manager->current_slide->text_lines))
        {
            SlideText text_line = kv_A(manager->current_slide->text_lines, ++manager->current_text_index);
            manager->text_duration = text_line.duration; // Update the duration for the new line
            manager->current_text_duration = 0.0f;
            manager->text_fade_alpha = 0.0f;
        }
    }

    // Check if the current slide has finished
    if (manager->current_duration < manager->total_duration)
        return;

    // Move to the next slide if available
    if (manager->current_slide_index + 1 < kv_size(manager->slides))
    {
        manager->current_slide = kv_A(manager->slides, ++manager->current_slide_index);
        manager->texture_fade_alpha = 0.0f;
        manager->total_duration = manager->current_slide->total_duration;
        manager->current_duration = 0.0f;

        manager->current_text_index = 0;
        manager->current_text_duration = 0.0f;
        // manager->text_duration = 1.0f;
        manager->text_fade_alpha = 0.0f;
        TraceLog(LOG_INFO, "Total Duration %f", manager->total_duration);
    }
    else
    {
        manager->slides_end = true;
    }
}

void cleanup_slide(GameSlide *slide)
{
    for (int i = 0; i < kv_size(slide->text_lines); i++)
    {
        SlideText text = kv_A(slide->text_lines, i);
        if (text.text != NULL)
        {
            free(text.text); // Free each text string
        }
    }

    kv_destroy(slide->text_lines);
    free(slide);
}

void cleanup_slide_manager(SlideManager *self)
{
    if (self == NULL)
    {
        return;
    }

    for (int i = 0; i < kv_size(self->slides); i++)
    {
        GameSlide *slide = kv_A(self->slides, i);
        cleanup_slide(slide); // Clean each individual slide
    }

    kv_destroy(self->slides);
    free(self);
}

SlideManager *create_slide_manager(const char *slides_scene_name)
{
    SlideManager *slide_manager = (SlideManager *)malloc(sizeof(SlideManager));
    if (slide_manager == NULL)
    {
        return NULL;
    }

    kv_init(slide_manager->slides);
    slide_manager->font = resource_manager.get_pixel7_font();

    JSON_Value *root_value = json_parse_file(settings.file_locations.slides_file);
    if (root_value == NULL)
    {
        free(slide_manager); // Clean up if parsing fails
        return NULL;
    }

    JSON_Object *root_object = json_value_get_object(root_value);
    JSON_Array *intro_array = json_object_get_array(root_object, slides_scene_name); // Use parameter

    for (size_t i = 0; i < json_array_get_count(intro_array); i++)
    {
        JSON_Object *slide_obj = json_array_get_object(intro_array, i); // Updated name for clarity

        GameSlide *slide = create_game_slide(slide_obj);
        if (slide != NULL) // Ensure slide creation was successful
        {
            kv_push(GameSlide *, slide_manager->slides, slide);
        }
    }

    slide_manager->current_slide_index = 0;
    slide_manager->current_slide = kv_A(slide_manager->slides, slide_manager->current_slide_index);

    slide_manager->texture_fade_alpha = 0.0f;
    slide_manager->texture_fade_color = (Color){255, 255, 255, (int)slide_manager->texture_fade_alpha};

    slide_manager->total_duration = slide_manager->current_slide->total_duration;
    slide_manager->current_duration = 0.0f;

    slide_manager->slides_end = false;

    slide_manager->text_fade_alpha = 0.0f;
    slide_manager->text_fade_color = (Color){255, 255, 255, (int)slide_manager->text_fade_alpha};
    slide_manager->current_text_index = 0;
    slide_manager->current_text_duration = 0.0f;
    SlideText text_line = kv_A(slide_manager->current_slide->text_lines, slide_manager->current_text_index);
    slide_manager->text_duration = text_line.duration;

    slide_manager->render = render;
    slide_manager->update = update;
    slide_manager->cleanup = cleanup_slide_manager;

    json_value_free(root_value); // Clean up the parsed JSON
    return slide_manager;
}