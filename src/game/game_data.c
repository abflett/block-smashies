#include <string.h> // For strcpy
#include "game_data.h"

static GameData game_data;

GameData *create_game_data(const char *name)
{
    // Set the Team's name
    strncpy(game_data.name, name, sizeof(game_data.name) - 1);
    game_data.name[sizeof(game_data.name) - 1] = '\0'; // Ensure null termination
    game_data.lives = 3;                               // Starting lives

    // Initialize player progress
    game_data.operation_complete = 0;
    game_data.mission_complete = 0;
    game_data.currency = 0; // Starting currency
    game_data.currency_collected = 0;

    // Initialize paddle attributes
    game_data.paddle.acceleration = 300.0f;
    game_data.paddle.max_velocity = 200.0f;
    game_data.paddle.friction = 0.97f;
    game_data.paddle.charge = 0.1f;
    game_data.paddle.booster_str = 200.0f;
    game_data.paddle.pulse_str = 100.0f;

    // Initialize ball attributes
    game_data.ball.speed_multiplier = 1.0f;
    game_data.ball.max_velocity = 50.0f;
    game_data.ball.min_velocity = 20.0f;
    game_data.ball.power = 6.25f; // Starting power level

    // Initialize perks
    game_data.perks.phase_shift = 0.0f;
    game_data.perks.phase_nova = 0.0f;
    game_data.perks.super_nova = 0.0f;
    game_data.perks.time_manipulation = false;
    game_data.perks.orb_shot = false;

    return &game_data;
}
