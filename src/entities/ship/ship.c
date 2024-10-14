#include "ship.h"

static int calculate_segments_func(Ship *ship)
{
    TraceLog(LOG_INFO, "Player: %d, Count: %d", *ship->player, *ship->player_count);
    ship->previous_player_count = *ship->player_count;
    switch (*ship->player_count)
    {
    case 1:
        return 4; // Single player, 4 segments
    case 2:
        return 2; // 2 players, each with 2 segments
    case 3:
        return (*ship->player == 0) ? 2 : 1; // Player 1 has 2, others have 1
    case 4:
        return 1; // 4 players, each with 1 segment
    default:
        return 1; // Fallback, should not happen unless player_count is misconfigured
    }
}

static void move_ship(struct Ship *ship, b2Vec2 position) {}
static void update_ship(struct Ship *ship, float delta_time) {}

static void render_ship(struct Ship *ship)
{
    if (ship->previous_player_count != *ship->player_count)
    {
        ship->segments = calculate_segments_func(ship);
    }
    ship->ship_body->render(ship->ship_body);
    ship->ship_shield->render(ship->ship_shield);
    ship->ship_thrusters->render(ship->ship_thrusters);
}

static void disable_ship(struct Ship *ship) {}
static void reset_ship(struct Ship *ship, b2Vec2 position) {}
static void cleanup_ship(struct Ship *ship)
{

    ship->ship_body->cleanup(ship->ship_body);
    ship->ship_shield->cleanup(ship->ship_shield);
    ship->ship_thrusters->cleanup(ship->ship_thrusters);
    free(ship);
}

Ship *create_ship(int *player, int *player_count, int *ship_color, int *shield_level, b2Vec2 position)
{
    Ship *ship = malloc(sizeof(Ship));

    ship->active = true;
    ship->player = player;
    ship->player_count = player_count;
    ship->previous_player_count = *player_count;
    ship->ship_color = ship_color;
    ship->position = position;
    ship->shield_level = shield_level;

    ship->segments = calculate_segments_func(ship);

    ship->ship_body = create_ship_body(&ship->segments, ship->ship_color, &ship->position);
    ship->ship_shield = create_ship_shield(&ship->segments, ship->shield_level, &ship->position);
    ship->ship_thrusters = create_ship_thrusters(&ship->segments, &ship->position);

    ship->calculate_segments = calculate_segments_func;
    ship->move = move_ship;
    ship->update = update_ship;
    ship->render = render_ship;
    ship->disable = disable_ship;
    ship->reset = reset_ship;
    ship->cleanup = cleanup_ship;

    return ship;
}
