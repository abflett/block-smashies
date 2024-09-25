#include <string.h> // For strcpy
#include "game_data.h"

GameData create_game_data(const char *name)
{
    GameData new_player;

    // Set the player's name
    strncpy(new_player.name, name, sizeof(new_player.name) - 1);
    new_player.name[sizeof(new_player.name) - 1] = '\0'; // Ensure null termination
    new_player.lives = 3;                                // Starting lives

    // Initialize player progress
    new_player.operation_complete = 0;
    new_player.mission_complete = 0;
    new_player.currency = 0; // Starting currency
    new_player.currency_collected = 0;

    // Initialize paddle attributes
    new_player.paddle.acceleration = 300.0f;
    new_player.paddle.max_velocity = 200.0f;
    new_player.paddle.friction = 0.97f;
    new_player.paddle.charge = 0.1f;
    new_player.paddle.booster_str = 200.0f;
    new_player.paddle.pulse_str = 100.0f;

    // Initialize ball attributes
    new_player.ball.speed_multiplier = 1.0f;
    new_player.ball.max_velocity = 50.0f;
    new_player.ball.min_velocity = 20.0f;
    new_player.ball.power = 6.25f; // Starting power level

    // Initialize perks
    new_player.perks.phase_shift = 0.0f;
    new_player.perks.phase_nova = 0.0f;
    new_player.perks.super_nova = 0.0f;
    new_player.perks.time_manipulation = false;
    new_player.perks.orb_shot = false;

    return new_player;
}
