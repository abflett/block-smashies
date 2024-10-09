#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "shake_effect.h"
#include "input_manager.h"

typedef struct Game
{
    InputManager *input_manager;
    void (*run)(void);
} Game;

Game *create_game(const char *game_title);
void exit_game(void);
ShakeEffect *get_shake_effect(void);

#endif