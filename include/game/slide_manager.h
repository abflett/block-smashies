#ifndef SLIDE_MANAGER_H
#define SLIDE_MANAGER_H

#include "raylib.h"
#include "game_slide.h"
#include "kvec.h"

typedef struct SlideManager
{
    Font *font;

    kvec_t(GameSlide *) slides;
    GameSlide *current_slide;
    int current_slide_index;  // selected slide index
    float texture_fade_alpha; // increment alpha for the fade color, for the fade in effect.
    bool slides_end;          // flag true when no more slides

    int current_text_index;
    float current_text_duration; // start at 0 and compare against text_duration
    float text_duration;         // the current text line duration
    float text_fade_alpha;       // increment alpha for the fade color, for the fade in effect.

    void (*render)(const struct SlideManager *manager);
    void (*update)(struct SlideManager *manager, float delta_time);
    void (*cleanup)(struct SlideManager *manager);
} SlideManager;

SlideManager *create_slide_manager(const char *slides_scene_name);

#endif
