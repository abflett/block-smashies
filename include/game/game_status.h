#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include <stdbool.h>
#include "player_stat.h"

typedef struct GameStatus
{
    PlayerStat player_stats[4];
    int player_count;
    int enemy_count;
    int score;
    float currency;
    int lives;
    float game_time;
    bool game_over; // flag true when no more lives
    bool is_hold;   // hold or play starting animation sequence
    bool is_pause;  // pause game on menu state etc
    void (*update)(float delta_time);
    void (*reset)(void);
} GameStatus;

GameStatus *create_game_status(int player_count);

#endif
