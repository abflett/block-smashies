#include "game_slide.h"
#include "resource_manager.h"

GameSlide *create_game_slide(JSON_Object *slide_obj)
{
    GameSlide *slide = (GameSlide *)malloc(sizeof(GameSlide));
    kv_init(slide->text_lines);

    const char *image = json_object_get_string(slide_obj, "image");
    const char *audio = json_object_get_string(slide_obj, "audio");

    JSON_Array *text_lines_array = json_object_get_array(slide_obj, "text_lines");

    slide->total_duration = 0.0f;

    // Iterate over the text_lines array
    for (int i = 0; i < json_array_get_count(text_lines_array); i++)
    {
        JSON_Object *text_line_obj = json_array_get_object(text_lines_array, i);
        JSON_Array *text_array = json_object_get_array(text_line_obj, "text"); // Get the array of text strings
        float duration = (float)json_object_get_number(text_line_obj, "duration");

        // For each line in the text array
        for (int j = 0; j < json_array_get_count(text_array); j++)
        {
            const char *text = json_array_get_string(text_array, j);
            if (text != NULL)
            {
                SlideText slide_text;
                slide_text.text = strdup(text); // Copy the text
                slide_text.duration = duration;

                slide->total_duration += slide_text.duration;
                kv_push(SlideText, slide->text_lines, slide_text);
            }
            else
            {
                TraceLog(LOG_WARNING, "Null text at index %d in slide %d", j, i);
            }
        }
    }

    slide->texture = &resource_manager.get_texture(image)->texture;

    TraceLog(LOG_INFO, "texture: %p", slide->texture);
    slide->audio = NULL; // Handle audio if necessary

    slide->current_line_index = 0;
    slide->line_duration_timer = 0.0f; // slide manager will handle this and update and compare to the SlideText duration

    return slide;
}
