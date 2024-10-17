#ifndef GAME_H
#define GAME_H

typedef struct Game
{
    void (*run)(void);
} Game;

Game *create_game(const char *game_title);
void exit_game(void);

#endif