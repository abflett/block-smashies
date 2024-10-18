#ifndef SLIDE_MANAGER_H
#define SLIDE_MANAGER_H

#include "raylib.h"
#include "game_slide.h"
#include "kvec.h"

typedef struct SlideManager
{
    kvec_t(GameSlide *) slides;
    GameSlide *current_slide;
    int current_slide_index;  // selected slide index
    bool slides_end;          // flag true when no more slides
    Color texture_fade_color; // color of the texture, modified by texture_fade_alpha
    float texture_fade_alpha; // increment alpha for the fade color, for the fade in effect.
    float total_duration;     // switch to next slide after this time
    float current_duration;   // current duration to compare against total

    Font *font;

    int current_text_index;
    float current_text_duration; // start at 0 and compare against text_duration
    float text_duration;         // the current text line duration
    Color text_fade_color;       // color of the current text line, modified by text_fade_alpha
    float text_fade_alpha;       // increment alpha for the fade color, for the fade in effect.

    void (*render)(struct SlideManager *manager);
    void (*update)(struct SlideManager *manager, float delta_time);
    void (*cleanup)(struct SlideManager *manager);
} SlideManager;

SlideManager *create_slide_manager(const char *slides_scene_name); // read slides from json file and store in kvec

#endif
