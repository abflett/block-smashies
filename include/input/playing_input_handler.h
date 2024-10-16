#ifndef PLAYING_INPUT_HANDLER_H
#define PLAYING_INPUT_HANDLER_H

#include "game_context.h"

typedef struct PlayingInputHandler
{
    GameContext *context;
    void (*update)(void);
} PlayingInputHandler;

PlayingInputHandler *initialize_playing_input(GameContext *context);

#endif