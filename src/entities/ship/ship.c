#include "ship.h"

static void move_ship(b2Vec2 position) {}    // movement for animation scenes without physics
static void update_ship(float delta_time) {} // handle player input and other updates
static void render_ship(void) {}
static void disable_ship(void) {}          // set active false
static void reset_ship(b2Vec2 position) {} // reset position on death etc.
static void cleanup_ship(void) {}

static int calculate_segments(int player, int player_count)
{
    switch (player_count)
    {
    case 1:
        return 4; // Single player, 4 segments
    case 2:
        return 2; // 2 players, each with 2 segments
    case 3:
        return (player == 1) ? 2 : 1; // Player 1 has 2, others have 1
    case 4:
        return 1; // 4 players, each with 1 segment
    default:
        return 1; // Fallback, should not happen unless player_count is misconfigured
    }
}

Ship *create_ship(int player, int player_count, int ship_color, b2Vec2 position)
{
    Ship *ship = malloc(sizeof(Ship));

    ship->active = true;
    ship->player = player;
    ship->player_count = player_count;
    ship->ship_color = ship_color;
    ship->position = position;

    ship->segments = calculate_segments(ship->player, ship->player_count);
    ShipBody *shipbody = create_ship_body(&ship->segments, &ship_color, &ship->position);
    //  ship->ship_body = create_ship_body(&ship->segments, &ship_color, &ship->position);
    //   ship->ship_shield = create_ship_shield();
    //   ship->ship_thrusters = create_ship_thrusters();

    ship->move = move_ship;
    ship->update = update_ship;
    ship->render = render_ship;
    ship->disable = disable_ship;
    ship->reset = reset_ship;
    ship->cleanup = cleanup_ship;

    return ship;
}
