#include "ship.h"
#include "entity_type.h"
#include "game_context.h"
#include "collision_category.h"

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
    if (ship->physics_active)
    {
        ship->position = b2Body_GetPosition(ship->body);
    }

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

void activate_ship_physics(Ship *ship, GameContext *game_context)
{
    TraceLog(LOG_INFO, "Activating ship physics");
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = ship->position; // set position to ship position
    body_def.linearDamping = 3.0f;
    body_def.isBullet = true;
    ship->body = b2CreateBody(game_context->world_id, &body_def);

    b2Vec2 shield_size = {ship->ship_shield->subtexture->src.width * 0.5f, ship->ship_shield->subtexture->src.height * 0.5f};
    b2Polygon shield_box = b2MakeBox(shield_size.x, shield_size.y);

    b2ShapeDef shield_shape_def = b2DefaultShapeDef();
    shield_shape_def.density = 1.0f;
    shield_shape_def.friction = 0.1f;
    shield_shape_def.restitution = 1.0f; // High restitution for bouncing

    shield_shape_def.filter.categoryBits = CATEGORY_PADDLE;
    shield_shape_def.filter.maskBits = PADDLE_COLLIDE_WITH;

    b2CreatePolygonShape(ship->body, &shield_shape_def, &shield_box);

    // Create a static reference body for the prismatic joint
    b2BodyDef static_body_def = b2DefaultBodyDef();
    static_body_def.type = b2_staticBody;
    static_body_def.position = (b2Vec2){(settings.game.play_area.width / 2) + settings.game.play_area.x, 19.0f}; // Static reference point
    ship->constraint = b2CreateBody(game_context->world_id, &static_body_def);

    // Prismatic Joint: Constrain movement to the X-axis
    b2PrismaticJointDef x_joint_def = b2DefaultPrismaticJointDef();
    x_joint_def.bodyIdA = ship->body;
    x_joint_def.bodyIdB = ship->constraint;
    x_joint_def.localAxisA = (b2Vec2){1.0f, 0.0f}; // Constrain to X-axis
    x_joint_def.enableLimit = false;               // No strict limits along X-axis
    x_joint_def.enableMotor = false;
    b2CreatePrismaticJoint(game_context->world_id, &x_joint_def);

    b2MassData mass_data = b2Body_GetMassData(ship->body);
    mass_data.mass = settings.gameplay.paddle_mass;
    b2Body_SetMassData(ship->body, mass_data);

    ship->physics_active = true;
    b2Body_SetUserData(ship->body, ship);
}

Ship *create_ship(int *player, GameData *game_data, b2Vec2 position)
{
    Ship *ship = malloc(sizeof(Ship));

    ship->type = ENTITY_PADDLE;
    ship->active = false;
    ship->physics_active = false;
    ship->player = player;
    ship->player_count = &game_data->player_count;
    ship->previous_player_count = *ship->player_count;
    ship->ship_color = &game_data->ships[*ship->player].ship_color;
    ship->position = position;
    ship->shield_level = &game_data->ships[*ship->player].shield_level;
    ship->segments = calculate_segments_func(ship);

    ship->ship_body = create_ship_body(&ship->segments, ship->ship_color, &ship->position);
    ship->ship_shield = create_ship_shield(&ship->segments, ship->shield_level, &ship->position);
    ship->ship_thrusters = create_ship_thrusters(&ship->segments, &ship->position);

    // add box2d data
    // fuction to set physics body to ship and all settings

    ship->activate_ship_physics = activate_ship_physics;
    ship->calculate_segments = calculate_segments_func;
    ship->move = move_ship;
    ship->update = update_ship;
    ship->render = render_ship;
    ship->disable = disable_ship;
    ship->reset = reset_ship;
    ship->cleanup = cleanup_ship;

    return ship;
}
