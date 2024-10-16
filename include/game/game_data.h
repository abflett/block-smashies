#ifndef GAME_DATA_H
#define GAME_DATA_H

#define MAX_SHIPS 4

#include <stdbool.h>
#include "uuid4.h"

/*  GameData
    - Used for saving data that will remain persistant
    - Using the upgrade tree will modify and save the new game_data
*/

typedef enum
{
    NOVICE_DIFFICULTY,
    EASY_DIFFICULTY,
    NORMAL_DIFFICULTY,
    HARD_DIFFICULTY,
    INSANE_DIFFICULTY
} Difficulty;

typedef struct ShipCustomization
{
    bool active;    // active based on player count
    int ship_color; // ship color
    int player_num; // player number: shape/segments based on player_num and player_count
    int shield_level;
} ShipCustomization;

typedef struct PaddleAttributes
{
    // Ship Attributes
    int shield_level;
    float force;          // general movement force
    float friction;       // ball manipulation
    float damping;        // de-acceleration - affects max velocity as well
    float max_energy;     // max_energy
    float boost_force;    // boost force - horizontal burst
    float boost_cooldown; // boost cooldown timer < is better
    float pulse_force;    // boost force - vertical burst
    float pulse_cooldown; // pulse cooldown timer < is better
    float max_heat;       // heat buildup % < is no heat
} PaddleAttributes;

typedef struct BallAttributes
{
    float max_velocity; // max velocity
    float min_velocity; // min velocity
    float power;        // damage
} BallAttributes;

typedef struct Perks
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
    char uuid[UUID4_LEN]; // game_data id for saving and loading
    char name[15];        // Team name max 10 chars
    int player_count;
    int operation;
    int mission;            // 5 missions per operation
    int currency;           // available currency
    int currency_collected; // total currency collected
    int high_score;         // current high score
    int lives;              // number of orb catchers/lives

    // Difficulty and modifiers
    Difficulty difficulty;
    int max_lives;         // max ammount of saver bots
    float difficulty_high; // 0.5 - 2.0 - currency * difficulty_high > is easier
    float difficulty_low;  // 0.5 - 2.0 - ball.max_velocity * difficulty_low < is easier

    PaddleAttributes paddle;
    BallAttributes ball;
    Perks perks;
    ShipCustomization ships[MAX_SHIPS];

    void (*add_player)(void);    // change player count and update active ships
    void (*remove_player)(void); // change player cound and update active ships
    void (*save_data)(void);     // save game data to the json file, either updating or creating a new json object, uses id of current game_data global struct
} GameData;

GameData *create_game_data(void);           // create a new default game data with starting attributes
GameData *load_game_data(const char *uuid); // create a game data from an existing object from json

#endif
