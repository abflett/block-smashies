#include "game_slide.h"
#include "resource_manager.h"

GameSlide *create_game_slide(const JSON_Object *slide_obj)
{
    GameSlide *slide = malloc(sizeof(GameSlide));
    kv_init(slide->text_lines);

    const char *image = json_object_get_string(slide_obj, "image");
    // Todo: add audio
    //const char *audio = json_object_get_string(slide_obj, "audio");

    const JSON_Array *text_lines_array = json_object_get_array(slide_obj, "text_lines");

    slide->total_duration = 0.0f;

    // Iterate over the text_lines array
    for (int i = 0; i < json_array_get_count(text_lines_array); i++)
    {
        const JSON_Object *text_line_obj = json_array_get_object(text_lines_array, i);

        const char *text = json_object_get_string(text_line_obj, "text"); // Get the array of text strings
        const float duration = (float)json_object_get_number(text_line_obj, "duration");

        SlideText slide_text;
        slide_text.text = strdup(text); // Copy the text
        slide_text.duration = duration;
        slide->total_duration += slide_text.duration;
        kv_push(SlideText, slide->text_lines, slide_text);
    }

    slide->texture = &resource_manager.get_texture(image)->texture;
    slide->audio = NULL; // Handle audio if necessary

    return slide;
}
