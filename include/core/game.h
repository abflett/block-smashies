#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "shake_effect.h"

typedef struct Game
{
    void (*run)(void);
} Game;

Game *create_game(const char *game_title);
void exit_game(void);
ShakeEffect *get_shake_effect(void);

#endif