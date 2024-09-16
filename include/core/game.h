#ifndef GAME_H
#define GAME_H

#include "raylib.h"

typedef struct Game
{
    void (*run)(void);
} Game;

Game *create_game(const char *game_title, const char *settings_filename, const char *resource_filename);
void exit_game(void);

#endif