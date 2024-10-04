#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdbool.h>
#include <ship.h>

/*  GameData
    Used for saving data that will remain persistant
*/

typedef struct
{
    // Ship Attributes
    float force;          // general movement force
    float friction;       // ball manipulation
    float damping;        // de-acceleration - affects max velocity as well
    float max_energy;     // max_energy
    float boost_force;    // boost force - horizontal burst
    float boost_cooldown; // boost cooldown timer < is better
    float pulse_force;    // boost force - vertical burst
    float pulse_cooldown; // pulse cooldown timer < is better
    float heat;           // heat buildup % < is no heat
} PaddleAttributes;

typedef struct
{
    float max_velocity; // max velocity
    float min_velocity; // min velocity
    float power;        // damage
} BallAttributes;

typedef struct
{
    // paddle perks
    float phase_shift; // dublicate orb weapon % chance
    bool orb_shot;     // consume heat and energy to fire bullets

    // ball perks
    bool time_manipulation; // slow ball.max_velocity duration
    float phase_nova;       // spawn explosion radius % chance
    float super_nova;       // max damage destroying any block duration
} Perks;

typedef struct GameData
{
    char name[11];
    int player_count;
    int operation_complete;
    int mission_complete;
    int currency;           // available currency
    int currency_collected; // total currency collected
    int high_score;         // current high score
    int lives;

    // Difficulty and modifiers
    int max_lives;         // max ammount of saver bots
    float difficulty_high; // 0.5 - 2.0 - currency * difficulty_high > is easier
    float difficulty_low;  // 0.5 - 2.0 - ball.max_velocity * difficulty_low < is easier

    PaddleAttributes paddle;
    BallAttributes ball;
    Perks perks;

    Ship *ships[4];

    void (*cleanup)(void);
    void (*add_player)(void);
    void (*remove_player)(void);
} GameData;

GameData *create_game_data(const char *name);

#endif
