#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdbool.h>

typedef struct
{
    float acceleration;
    float max_velocity;
    float friction;
    float charge;
    float booster_str;
    float pulse_str;
} PaddleAttributes;

typedef struct
{
    float speed_multiplier;
    float max_velocity;
    float min_velocity;
    float power;
} BallAttributes;

typedef struct
{
    float phase_shift;
    float phase_nova;
    float super_nova;
    bool time_manipulation;
    bool orb_shot;
} Perks;

typedef struct GameData
{
    char name[32];
    int operation_complete;
    int mission_complete;
    int currency;
    int currency_collected;
    int lives;

    PaddleAttributes paddle;
    BallAttributes ball;
    Perks perks;

} GameData;

GameData *create_game_data(const char *name);

#endif
