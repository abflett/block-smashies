#include <string.h> // For strcpy
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
    game_data.lives = 1;              // number of orb catchers/lives

    game_data.difficulty = NORMAL_DIFFICULTY;
    game_data.max_lives = 5;          // max ammount of saver bots
    game_data.difficulty_high = 1.0f; // 0.5 - 2.0 - currency * difficulty_high > is easier
    game_data.difficulty_low = 1.0f;  // 0.5 - 2.0 - ball.max_velocity * difficulty_low < is easier

    // Initialize paddle attributes
    game_data.paddle.force = 300.0f;        // general movement force
    game_data.paddle.friction = 0.97f;      // ball manipulation
    game_data.paddle.damping = 3.0f;        // de-acceleration - affects max velocity as well
    game_data.paddle.max_energy = 100.0f;   // max energy
    game_data.paddle.boost_force = 200.0f;  // boost force - horizontal burst
    game_data.paddle.boost_cooldown = 2.0f; // boost cooldown timer < is better
    game_data.paddle.pulse_force = 100.0f;  // boost force - vertical burst
    game_data.paddle.pulse_cooldown = 2.0f; // pulse cooldown timer < is better
    game_data.paddle.max_heat = 100.0f;     // max_heat until over heat

    // Initialize ball attributes
    game_data.ball.max_velocity = 50.0f;
    game_data.ball.min_velocity = 20.0f;
    game_data.ball.power = 1.0f; // Starting power level

    // Initialize perks
    game_data.perks.phase_shift = 0.0f;
    game_data.perks.phase_nova = 0.0f;
    game_data.perks.super_nova = 0.0f;
    game_data.perks.time_manipulation = false;
    game_data.perks.orb_shot = false;

    for (int i = 0; i < MAX_SHIPS; i++)
    {
        game_data.ships[i] = (ShipCustomization){
            .active = i == 0, // set player 0 to active
            .player_num = i,
            .ship_color = 4,    // set default color to orange
            .shield_level = 0}; // set default shield level to 0
    }

    return &game_data;
}

void update_game_data(const char *uuid)
{
    // load json data from uuid
    // update game_data with loaded data
}
