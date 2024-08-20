#include <string.h> // For strcpy
#include "player.h"

Player create_new_player(const char *name)
{
    Player new_player;

    // Set the player's name
    strncpy(new_player.name, name, sizeof(new_player.name) - 1);
    new_player.name[sizeof(new_player.name) - 1] = '\0'; // Ensure null termination

    // Initialize player progress
    new_player.operation_complete = 0;
    new_player.mission_complete = 0;
    new_player.currency = 0; // Starting currency

    // Initialize paddle attributes
    new_player.paddle.acceleration = 600.0f;
    new_player.paddle.max_speed = 300.0f;
    new_player.paddle.friction = 0.95f;
    new_player.paddle.charge = 0.0f;
    new_player.paddle.booster_str = 1.0f;
    new_player.paddle.pulse_str = 1.0f;
    new_player.paddle.lives = 3; // Starting lives

    // Initialize ball attributes
    new_player.ball.speed_multiplier = 1.0f;
    new_player.ball.power = 1; // Starting power level

    // Initialize perks
    new_player.perks.phase_shift = 0.0f;
    new_player.perks.phase_nova = 0.0f;
    new_player.perks.super_nova = 0.0f;
    new_player.perks.time_manipulation = false;
    new_player.perks.orb_shot = false;

    return new_player;
}
