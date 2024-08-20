#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

typedef struct
{
    float acceleration;
    float max_speed;
    float friction;
    float charge;
    float booster_str;
    float pulse_str;
    int lives;
} PaddleAttributes;

typedef struct
{
    float speed_multiplier;
    int power;
} BallAttributes;

typedef struct
{
    float phase_shift;
    float phase_nova;
    float super_nova;
    bool time_manipulation;
    bool orb_shot;
} Perks;

typedef struct Player
{
    char name[32];
    int operation_complete;
    int mission_complete;
    int currency;

    PaddleAttributes paddle;
    BallAttributes ball;
    Perks perks;

} Player;

Player create_new_player(const char *name)

#endif
