#include <string.h>
#include "game_data.h"
#include "raylib.h"
#include "b_utils.h"
#include "uuid4.h"

static GameData game_data;

GameData *create_game_data(void)
{
    const char *uuid = create_uuid(); // Use your existing create_uuid function
    strncpy(game_data.uuid, uuid, UUID4_LEN);

    const char *name = "Team1";
    strncpy(game_data.name, name, sizeof(game_data.name) - 1);
    game_data.name[sizeof(game_data.name) - 1] = '\0'; // Ensure null termination

    game_data.num_purchased_upgrades = 5; // No upgrades unlocked at the start
    for (int i = 0; i < MAX_UPGRADES; i++)
    {
        game_data.purchased_upgrades[i] = -1; // -1 for no upgrade
    }

    // Todo: remove testing values
    game_data.purchased_upgrades[0] = 0; // purchased for testing
    game_data.purchased_upgrades[1] = 1; // purchased for testing
    game_data.purchased_upgrades[2] = 2; // purchased for testing
    game_data.purchased_upgrades[3] = 8; // purchased for testing
    game_data.purchased_upgrades[4] = 9; // purchased for testing

    game_data.player_count = 1;
    game_data.operation = 0;
    game_data.mission = 0;            // 5 missions per operation
    game_data.currency = 0;           // available currency
    game_data.currency_collected = 0; // total currency collected
    game_data.high_score = 0;         // current high score
    game_data.orb_retrievers = 1;     // number of orb catchers/orb_retrievers

    game_data.difficulty = NORMAL_DIFFICULTY;
    game_data.max_retrievers = 5;     // max amount of saver bots
    game_data.difficulty_high = 1.0f; // 0.5 - 2.0 - currency * difficulty_high > is easier
    game_data.difficulty_low = 1.0f;  // 0.5 - 2.0 - ball.max_velocity * difficulty_low < is easier

    // Initialize ship_attributes attributes
    game_data.ship_attributes.ship_force = 300.0f;   // general movement ship_force
    game_data.ship_attributes.orb_control = 0.97f;   // ball manipulation
    game_data.ship_attributes.ship_damping = 3.0f;   // de-acceleration - affects max velocity as well
    game_data.ship_attributes.max_energy = 100.0f;   // max energy
    game_data.ship_attributes.boost_force = 200.0f;  // boost ship_force - horizontal burst
    game_data.ship_attributes.boost_cooldown = 2.0f; // boost cooldown timer < is better
    game_data.ship_attributes.pulse_force = 100.0f;  // boost ship_force - vertical burst
    game_data.ship_attributes.pulse_cooldown = 2.0f; // pulse cooldown timer < is better
    game_data.ship_attributes.max_heat = 100.0f;     // max_heat until overheat

    // Initialize ball attributes
    game_data.orb_attributes.max_velocity = 50.0f;
    game_data.orb_attributes.min_velocity = 20.0f;
    game_data.orb_attributes.orb_power = 1.0f; // Starting orb_power level

    // Initialize perks
    game_data.perks.phase_shift = 0.0f;
    game_data.perks.orb_shot = false;
    game_data.perks.phase_nova = 0.0f;
    game_data.perks.super_nova = 0.0f;

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        game_data.ships[i] = (ShipCustomization){
            .active = i == 0, // set player 0 to active
            .player_num = i,
            .ship_color = 4,       // set default color to orange
            .shield_strength = 0}; // set default shield level to 0
    }

    return &game_data;
}

void update_game_data(const char *uuid)
{
    // load json data from uuid
    // update game_data with loaded data
}
