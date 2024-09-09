#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include <stdbool.h>

typedef struct GameStatus
{
    int score;
    int lives;
    float game_time;
    bool game_over; // flag true when no more lives
    bool is_hold;   // hold or play starting animation sequence
    bool is_pause;  // pause game on menu state etc
    void (*update)(struct GameStatus *status, float delta_time);
    void (*reset)(struct GameStatus *status);
    void (*render)(const struct GameStatus *status);
} GameStatus;

GameStatus create_game_status(void);

#endif
