#include <string.h> // For strcpy
#include "game_data.h"

static GameData game_data;

static void cleanup_game_data(void)
{
    for (int i = 0; i < 4; i++)
    {
        game_data.ships[i]->cleanup(game_data.ships[i]);
    }
}
static void add_player_game_data(void) {}
static void remove_player_game_data(void) {}

GameData *create_game_data(const char *name)
{
    // Set the Team's name
    strncpy(game_data.name, name, sizeof(game_data.name) - 1);
    game_data.name[sizeof(game_data.name) - 1] = '\0'; // Ensure null termination
    game_data.player_count = 1;
    game_data.lives = 3; // Starting lives

    // Initialize player progress
    game_data.operation_complete = 0;
    game_data.mission_complete = 0;
    game_data.currency = 0; // Starting currency
    game_data.currency_collected = 0;

    // Initialize paddle attributes
    game_data.paddle.force = 300.0f;        // general movement force
    game_data.paddle.friction = 0.97f;      // ball manipulation
    game_data.paddle.damping = 3.0f;        // de-acceleration - affects max velocity as well
    game_data.paddle.max_energy = 100.0f;   // max energy
    game_data.paddle.boost_force = 200.0f;  // boost force - horizontal burst
    game_data.paddle.boost_cooldown = 2.0f; // boost cooldown timer < is better
    game_data.paddle.pulse_force = 100.0f;  // boost force - vertical burst
    game_data.paddle.pulse_cooldown = 2.0f; // pulse cooldown timer < is better
    game_data.paddle.heat = 0.0f;           // heat buildup % < is no heat

    // Initialize ball attributes
    game_data.ball.max_velocity = 50.0f;
    game_data.ball.min_velocity = 20.0f;
    game_data.ball.power = 6.25f; // Starting power level

    // Initialize perks
    game_data.perks.phase_shift = 0.0f;
    game_data.perks.phase_nova = 0.0f;
    game_data.perks.super_nova = 0.0f;
    game_data.perks.time_manipulation = false;
    game_data.perks.orb_shot = false;

    for (int i = 0; i < 4; i++)
    {
        game_data.ships[i] = create_ship(i + 1, game_data.player_count, GetRandomValue(0, 25), (b2Vec2){0, 0});
    }

    game_data.cleanup = cleanup_game_data;
    game_data.add_player = add_player_game_data;
    game_data.remove_player = remove_player_game_data;

    return &game_data;
}
