#ifndef GAME_SLIDE_H
#define GAME_SLIDE_H

#include "raylib.h"
#include "parson.h"
#include "kvec.h"

typedef struct SlideText
{
    char *text;
    float duration;
} SlideText;

typedef struct GameSlide
{
    kvec_t(SlideText) text_lines;

    Texture2D *texture;
    AudioStream *audio;

    float total_duration;      // Duration for which the slide is displayed
    int current_line_index;    // Index for the current line being displayed
    float line_duration_timer; // Timer for line fade-in
} GameSlide;

GameSlide *create_game_slide(JSON_Object *slide_obj);

#endif
