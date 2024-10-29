#ifndef GAME_DATA_H
#define GAME_DATA_H

#define MAX_SHIPS 4

#include <stdbool.h>
#include "uuid4.h"

#define MAX_UPGRADES 64
#define MAX_TEAM_NAME_LENGTH 15

/*  GameData
    - Used for saving data that will remain persistent
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
    int shield_strength;
} ShipCustomization;

typedef struct ShipAttributes
{
    // Ship Attributes
    int shield_strength;
    float ship_force;     // general movement ship_force
    float orb_control;    // ball manipulation
    float ship_damping;   // de-acceleration - affects max velocity as well
    float max_energy;     // max_energy
    float boost_force;    // boost ship_force - horizontal burst
    float boost_cooldown; // boost cooldown timer < is better
    float pulse_force;    // boost ship_force - vertical burst
    float pulse_cooldown; // pulse cooldown timer < is better
    float max_heat;       // heat buildup % < is no heat
} ShipAttributes;

typedef struct OrbAttributes
{
    float max_velocity; // max velocity
    float min_velocity; // min velocity
    float orb_power;    // damage
} OrbAttributes;

typedef struct Perks
{
    // ship perks
    float phase_shift; // duplicate orb weapon % chance
    float orb_shot;    // consume heat and energy to fire bullets

    // orb perks
    float phase_nova; // spawn explosion radius % chance
    float super_nova; // max damage destroying any block duration
} Perks;

typedef struct GameData
{
    char uuid[UUID4_LEN]; // game_data id for saving and loading
    char name[MAX_TEAM_NAME_LENGTH];
    int player_count;
    int operation;
    int mission;            // 5 missions per operation
    int currency;           // available currency
    int currency_collected; // total currency collected
    int high_score;         // current high score
    int orb_retrievers;     // number of orb retrievers/lives

    int purchased_upgrades[MAX_UPGRADES]; // Array to hold upgrade IDs
    int num_purchased_upgrades;

    // Difficulty and modifiers
    Difficulty difficulty;
    int max_retrievers;    // max amount of saver bots
    float difficulty_high; // 0.5 - 2.0 - currency * difficulty_high > is easier
    float difficulty_low;  // 0.5 - 2.0 - ball.max_velocity * difficulty_low < is easier

    ShipAttributes ship_attributes;
    OrbAttributes orb_attributes;
    Perks perks;
    ShipCustomization ships[MAX_SHIPS];
} GameData;

GameData *create_game_data(void);        // create a new default game data with starting attributes
void update_game_data(const char *uuid); // update game data from the json file uuid

#endif
