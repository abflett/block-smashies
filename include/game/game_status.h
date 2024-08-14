#ifndef GAME_STATUS_H
#define GAME_STATUS_H

typedef struct GameStatus
{
    int score;
    int lives;
    float game_time;
    void (*update)(struct GameStatus *status, float delta_time);
    void (*reset)(struct GameStatus *status);
    void (*render)(const struct GameStatus *status);
} GameStatus;

GameStatus create_game_status(void);

#endif
